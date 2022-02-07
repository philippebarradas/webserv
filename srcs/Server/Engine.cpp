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
#include "../Parse_request/parse_request.hpp"

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

// Check if port config is binded
bool	Engine::is_binded(int port_config)
{
	for (int i = 0; i < this->_i_server_binded; i++)
	{
		if (port_config == this->_port_binded[i])
			return (TRUE);
	}
	return (FALSE);
}

// Put a name to a socket
void	Engine::bind_socket(int listen_fd, const std::vector<Server> & src)
{
	int port_config = 0;

	std::istringstream(src[this->_i_server].getListen()) >> port_config;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port_config);
	std::cout << GREEN << "Port: " << port_config << std::endl << END;
	if (is_binded(port_config) == FALSE)
	{
		if (bind(listen_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0)
			throw std::runtime_error("[Error] Bind failed");
		this->_port_binded[this->_i_server_binded] = port_config;
		this->_i_server_binded++;
	}
}

// Make the socket passive, waiting to accept
void	Engine::listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Set remote addr and remote port
void	Engine::set_remote_var(struct sockaddr_in & addr_client)
{
	int i_remote_port = ntohs(addr_client.sin_port);

	std::stringstream ss;
	ss << i_remote_port;
	this->_remote_port = ss.str();
	this->_remote_addr = inet_ntoa(addr_client.sin_addr);
}

// Accept connexion and return socket accepted
int	Engine::accept_connexions(int listen_fd)
{
	struct sockaddr_in addr_client;
	int new_socket = 0;
	int client_len = sizeof(addr_client);

	new_socket = accept(listen_fd, (struct sockaddr *)&addr_client, (socklen_t *)&client_len);
	set_remote_var(addr_client);
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
	if (this != &rhs)
	{
		this->_addr = rhs._addr;
		this->_buff_send = rhs._buff_send;
		this->_epfd = rhs._epfd;
		this->_i_server = rhs._i_server;
		this->_i_server_binded = rhs._i_server_binded;
		this->_nbr_servers = rhs._nbr_servers;
		this->_port = rhs._port;
		this->_remote_addr = rhs._remote_addr;
		this->_remote_port = rhs._remote_port;
		this->_timeout = rhs._timeout;
		this->_valread = rhs._valread;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Engine::setup_socket_server(const std::vector<Server> & src)
{
	this->_port = 0, this->_i_server_binded = 0;
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

void	Engine::read_send_data(int fd, const std::vector<Server> & src,Parse_request & Xarse_head)
{
	Treat_request	request;
	std::string		file_body;
	std::string 	buff_send;
	Parse_request	parse_head;

	size_t	buff_size = 330000;
	char	buff[buff_size];
	int		valread = -1;
	int		nbr_bytes_send = 0;
	bool	is_valid = true;
	size_t	recv_len = 0;

	std::cout << GREEN <<"start _next_buffer_is_body " << parse_head._next_buffer_is_body << END << std::endl << std::endl;
	bzero(&buff, sizeof(buff));
    while (valread != 0 && is_valid == true)
	{
		valread = recv(fd, &buff[recv_len], buff_size - recv_len, 0);
		if (valread == -1)
			throw std::runtime_error("[Error] recv() failed");
		else
			recv_len += valread;
	
		std::cout << "-buf-\n-|" << BLUE << buff << END << "|-\n-end-" << std::endl;

		if (parse_head.buff_is_valid(buff) == 0
		||(parse_head._next_buffer_is_body == 1 && parse_head._request_body_size == 0))
			epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout);
		else
			is_valid = false;
		//std::cout << "end boucle = " << parse_head._next_buffer_is_body << std::endl;
	}
	if (parse_head._next_buffer_is_body == 1 && parse_head._request_body_size != 0)
		parse_head._next_buffer_is_body = 0;

	std::cout << GREEN <<"OUTSIDE _next_buffer_is_body " << parse_head._next_buffer_is_body << END << std::endl << std::endl;
	//std::cout << std::endl << std::endl << std::endl;
	//std::cout << std::endl << std::endl << std::endl;
	std::vector<Server>::const_iterator it;
	std::string port_str = static_cast<std::ostringstream*>( &(std::ostringstream() << this->_port))->str();

	int i_listen = 0;
	for (it = src.begin(); it != src.end(); it++, i_listen++)
		if ((*it).getListen() == port_str)
			break ;
	Cgi		obj_cgi(src.at(i_listen), parse_head, *this, request);
	//Cgi		obj_cgi2(src.at(i_listen), parse_head, *this, request);


	std::cout << "CGI" << std::endl;
	//std::cout << " --|" << buff << "|-end-" << std::endl;
 	if (valread != 0)
	{
		if (obj_cgi.is_file_cgi(parse_head.get_request("Path")) == TRUE)
		{

			obj_cgi.exec_cgi(obj_cgi.create_argv(src.at(i_listen).getRoot() + "/hello.php"),
			obj_cgi.convert_env(obj_cgi.getEnv()), parse_head, request);

			//std::cout << "STR = ||" << obj_cgi.getSend_content().c_str() << "||"<< std::endl;

			nbr_bytes_send = send(fd, obj_cgi.getSend_content().c_str(),
				obj_cgi.getSend_content().size(), 0);
			//close(fd);
		}
		else
		{
			buff_send = request.is_Treat_request(buff, src, this->_port, parse_head);
			if (parse_head.get_request("Method").compare("POST") == 0)
				//parse_head.get_request("method").compare("GET") == 0)
			{
				obj_cgi.exec_cgi(obj_cgi.create_argv(src.at(i_listen).getRoot() + "/env.php"),
				obj_cgi.convert_env(obj_cgi.getEnv()), parse_head, request);
				nbr_bytes_send = send(fd, obj_cgi.getSend_content().c_str(),
					obj_cgi.getSend_content().size(), 0);
			}
			else
			{
				nbr_bytes_send = send(fd, buff_send.c_str(), buff_send.size(), 0);
			}
		}
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	}
	//std::cout << " ----|" << parse_head.get_request("Expect:") << "|" << std::endl;
	//if (parse_head.get_request("Expect:").compare("100-continue") != 0)
	//{
		close(fd);
	//}
	//else
	//{
	//	parse_head.set_next_buffer_is_body(TRUE);
	//	std::cout << "du coup _next_buffer_is_body " << parse_head._next_buffer_is_body << std::endl;
	//}
	//if (parse_head.get_request("status").compare("200") != 0 ||
		//parse_head.get_request("Connection:").find("close") != std::string::npos)
	//close(fd);
	std::cout << GREEN <<"END _next_buffer_is_body " << parse_head._next_buffer_is_body << END << std::endl << std::endl;

}

void	Engine::loop_server(const std::vector<Server> & src)
{
	int nbr_connexions = 0, new_socket = 0, i = 0;
	Parse_request	parse_head;
	std::cout << "init parse_head" << std::endl;

	while (TRUE)
	{
		if ((nbr_connexions = epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (i = 0; i < nbr_connexions; i++)
		{
			if (is_listener(this->_fds_events[i].data.fd, this->_listen_fd, this->_nbr_servers, src))
			{
				new_socket = accept_connexions(this->_fds_events[i].data.fd);
				this->_fds_events[i].events = EPOLLIN;
				this->_fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket, &this->_fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				read_send_data(this->_fds_events[i].data.fd, src, parse_head);
				//close(this->_fds_events[i].data.fd);
			}
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string	Engine::GetRemote_Port() const
{
	return (this->_remote_port);
}

std::string	Engine::GetRemote_Addr() const
{
	return (this->_remote_addr);
}
/* ************************************************************************** */
