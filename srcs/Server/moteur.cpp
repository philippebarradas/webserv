/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moteur.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/17 18:58:04dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "moteur.hpp"
#include "../Cgi/Cgi.hpp"
#include "../method/method.hpp"
#include "../Parse_header/parse_header.hpp"

// Creating socket file descriptor
int	Moteur::create_socket()
{
	int _listen_fd = 0;
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (_listen_fd);
}

// Set socket file descriptor to be reusable
void	Moteur::set_socket(int listen_fd)
{
	int opt = 1;
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Put a name to a socket
void	Moteur::bind_socket(int listen_fd, const std::vector<Server> & src)
{
	struct sockaddr_in address;
	int port_config = 0;

	std::istringstream(src[this->_i_server].getListen()) >> port_config;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port_config);
	std::cout << GREEN << "Port: " << port_config << std::endl << END;
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("[Error] Port already attribute");
}

// Make the socket passive, waiting to accept
void	Moteur::listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Accept connexion and return socket accepted
int	Moteur::accept_connexions(int listen_fd)
{
	int new_socket = 0;

	new_socket = accept(listen_fd, NULL, NULL);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	return (new_socket);
}

// savoir si le fd dans le epoll est un listener (socket d'un port) ou non
bool	Moteur::is_listener(int fd, int *tab_fd, int nbr_servers, const std::vector<Server> & src)
{
	for (int i = 0; i < nbr_servers; i++)
	{
		if (fd == tab_fd[i])
		{
			std::istringstream(src[i].getListen()) >> this->_port;
			return (TRUE);
		}
	}
	return (FALSE);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Moteur::Moteur()
{
}

Moteur::Moteur(const std::vector<Server> & src)
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server(src);
	loop_server(src);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Moteur::~Moteur()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Moteur&				Moteur::operator=( Moteur const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Moteur::setup_socket_server(const std::vector<Server> & src)
{
	this->_port = 0;
	this->_nbr_servers = src.size();
	this->_timeout = 3 * 60 * 1000; // 3 min de _timeout (= keepalive nginx ?)
	this->_epfd = epoll_create(MAX_EVENTS);
	if (this->_epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	for (this->_i_server = 0; this->_i_server < this->_nbr_servers; this->_i_server++)
	{
		this->_listen_fd[this->_i_server] = create_socket();
		this->_fds_events[this->_i_server].data.fd = this->_listen_fd[this->_i_server];
		this->_fds_events[this->_i_server].events = EPOLLIN;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_listen_fd[this->_i_server], &this->_fds_events[this->_i_server]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->_listen_fd[this->_i_server]);
		bind_socket(this->_listen_fd[this->_i_server], src);
		listen_socket(this->_listen_fd[this->_i_server]);
	}
}

void	Moteur::read_data(int fd, const std::vector<Server> & src, Parse_header & parse_head)
{
	this->_valread = -1;
	bool	is_valid = true;
	size_t	old_len = 0, recv_len = 0;

	bzero(&this->_buff, sizeof(this->_buff));
	while (this->_valread != 0 && is_valid == true)
	{
		old_len = std::strlen(this->_buff);
		this->_valread = recv(fd, &this->_buff[recv_len], BUFFER_SIZE - recv_len, 0);
		if (this->_valread == -1)
			throw std::runtime_error("[Error] recv() failed");
		else
			recv_len += this->_valread;
		if (parse_head.buff_is_valid(this->_buff, this->_buff + old_len) == 0)
			epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout);
		else
			is_valid = false;
	}
	parse_head.display_content_header();
}

void	Moteur::send_data(int fd, const std::vector<Server> & src, const Parse_header & parse_head)
{
	std::string port_str = static_cast<std::ostringstream*>( &(std::ostringstream() << this->_port))->str();
	std::vector<Server>::const_iterator it;
	int i_listen = 0;
	for (it = src.begin(); it != src.end(); it++, i_listen++)
	{
		if ((*it).getListen() == port_str)
			break ;
	}
	Cgi		obj_cgi(src.at(i_listen), parse_head);
	int		nbr_bytes_send = 0;
	Method			meth;
	if (this->_valread != 0)
	{
		this->_buff_send = meth.is_method(this->_buff, src, this->_port, parse_head);
		if (obj_cgi.is_cgi(parse_head) == TRUE)
			nbr_bytes_send = send(fd, obj_cgi.getSend_content().c_str(),
				obj_cgi.getSend_content().size(), 0);
		else
			nbr_bytes_send = send(fd, _buff_send.c_str(), _buff_send.size(), 0);
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
		close(fd);
	}
	if (parse_head.get_request_status() != 200)
		close(fd);
}

void	Moteur::loop_server(const std::vector<Server> & src)
{
	Parse_header parse_head;
	int nbr_connexions = 0, new_socket = 0, i = 0;

	while (TRUE)
	{
		if ((nbr_connexions = epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (i = 0; i < nbr_connexions; i++)
		{
			if (is_listener(this->_fds_events[i].data.fd, this->_listen_fd, this->_nbr_servers, src))
			{
				new_socket = accept_connexions(this->_fds_events[i].data.fd);
				fcntl(new_socket, F_SETFL, O_NONBLOCK);
				this->_fds_events[i].events = EPOLLIN;
				this->_fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket, &this->_fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				read_data(this->_fds_events[i].data.fd, src, parse_head);
				send_data(this->_fds_events[i].data.fd, src, parse_head);
				//break ;
			}
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
