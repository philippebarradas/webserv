/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/17 18:58:04dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"
#include "../Treat_request/treat_request.hpp"
#include "../Cgi/Cgi.hpp"
#include "../Parse_header/parse_header.hpp"

// Creating socket file descriptor
int	Engine::create_socket()
{
	int _listen_fd = 0;
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (_listen_fd);
}

// Set socket file descriptor to be reusable
void	Engine::set_socket(int listen_fd)
{
	int opt = 1;
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Put a name to a socket
void	Engine::bind_socket(int listen_fd, const std::vector<Server> & src)
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
void	Engine::listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Accept connexion and return socket accepted
int	Engine::accept_connexions(int listen_fd)
{
	int new_socket = 0;

	new_socket = accept(listen_fd, NULL, NULL);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	return (new_socket);
}

// savoir si le fd dans le epoll est un listener (socket d'un port) ou non
bool	Engine::is_listener(int fd, int *tab_fd, int nbr_servers, const std::vector<Server> & src)
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

Engine::Engine()
{
}

Engine::Engine(const std::vector<Server> & src)
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server(src);
	loop_server(src);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Engine::~Engine()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Engine&				Engine::operator=( Engine const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Engine::setup_socket_server(const std::vector<Server> & src)
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

void	Engine::read_send_data(int fd, const std::vector<Server> & src)
{
	Treat_request	treat;
	Parse_header	parse_head;
	std::string		file_body;
	std::string 	buff_send;

	size_t	buff_size = 33000;
	char	buff[buff_size];
	int		valread = -1;
	int		nbr_bytes_send = 0;
	bool	is_valid = true;
	size_t	recv_len = 0;

	bzero(&buff, sizeof(buff));
    while (valread != 0 && is_valid == true)
	{
		valread = recv(fd, &buff[recv_len], buff_size - recv_len, 0);
		if (valread == -1)
			throw std::runtime_error("[Error] recv() failed");
		else
			recv_len += valread;
		if (parse_head.buff_is_valid(buff) == 0)
			epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout);
		else
			is_valid = false;
	}
	std::cout << std::endl << std::endl << std::endl;
	std::cout << std::endl << std::endl << std::endl;
	std::vector<Server>::const_iterator it;
	std::string port_str = static_cast<std::ostringstream*>( &(std::ostringstream() << this->_port))->str();

	int i_listen = 0;
	for (it = src.begin(); it != src.end(); it++, i_listen++)
		if ((*it).getListen() == port_str)
			break ;
	Cgi		obj_cgi(src.at(i_listen), parse_head);
 	if (valread != 0)
	{
		if (obj_cgi.is_file_cgi(parse_head.get_request("PATH")) == TRUE)
		{
			obj_cgi.exec_cgi(obj_cgi.create_argv(src.at(i_listen).getRoot() + "/env.php"),
			obj_cgi.convert_env(obj_cgi.getEnv()));
			nbr_bytes_send = send(fd, obj_cgi.getSend_content().c_str(),
				obj_cgi.getSend_content().size(), 0);
			close(fd);
		}
		else
		{
			buff_send = treat.is_Treat_request(buff, src, this->_port, parse_head);
			nbr_bytes_send = send(fd, buff_send.c_str(), buff_send.size(), 0);
		}
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	}
	if (parse_head.get_request("status").compare("200") != 0 ||
		parse_head.get_request("Connection:").find("close") != std::string::npos)
		close(fd);
}

void	Engine::loop_server(const std::vector<Server> & src)
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
				read_send_data(this->_fds_events[i].data.fd, src);
				//break ;
			}
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
