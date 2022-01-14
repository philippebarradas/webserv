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
#include "../method/method.hpp"
#include "../Parse_header/parse_header.hpp"

#include <stdlib.h>

// Creating socket file descriptor
int	Moteur::create_socket()
{
	int listen_fd = 0;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (listen_fd);
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

	std::istringstream(src[this->i_server].getListen()) >> port_config;
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
			std::istringstream(src[i].getListen()) >> this->port;
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
	this->port = 0;
	this->nbr_servers = src.size();
	this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	this->epfd = epoll_create(MAX_EVENTS);
	if (this->epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	for (this->i_server = 0; this->i_server < this->nbr_servers; this->i_server++)
	{
		this->listen_fd[this->i_server] = create_socket();
		this->fds_events[this->i_server].data.fd = this->listen_fd[this->i_server];
		this->fds_events[this->i_server].events = EPOLLIN;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->listen_fd[this->i_server], &fds_events[this->i_server]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->listen_fd[this->i_server]);
		bind_socket(this->listen_fd[this->i_server], src);
		listen_socket(this->listen_fd[this->i_server]);
	}
}



void	Moteur::read_send_data(int fd, const std::vector<Server> & src)
{
	int valread = -1;

	size_t buff_size = 1000;
	
	char buff[buff_size];


	Method meth;
	Parse_header parse_head;
	parse_head.set_nbr_line(0);
	

	size_t recv_len = 0;
	size_t x = 0;

	bzero(&buff, sizeof(buff));
	
    while (valread != 0)
	{
		valread = recv(fd, &buff[recv_len], buff_size - recv_len, 0);
		if (valread == -1)
			std::cout << "valread == -1" << std::endl;//	std::cout << "error" << std::endl;
		else
			recv_len += valread;

		std::cout << "buff = [" << buff << "]" << std::endl;

		if (parse_head.buff_is_valid(buff) == 0)	
			epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, this->timeout);
		else
			break;
	}

	//std::cout << "full buff = ((" << buff << "))" << std::endl;


	std::cout << "_host = [" << parse_head.get_host() << "]" << std::endl;
	std::cout << "_user_agent = [" << parse_head.get_user_agent() << "]" << std::endl;
	std::cout << "_accept = [" << parse_head.get_accept() << "]"<< std::endl;
	std::cout << "_accept_language = [" << parse_head.get_accept_language() << "]"<< std::endl;
	std::cout << "_accept_encoding = [" << parse_head.get_accept_encoding() << "]"<< "]"<< std::endl;
	std::cout << "_method_charset = [" << parse_head.get_method_charset() << std::endl;
	std::cout << "_keep_alive = [" << parse_head.get_keep_alive() << "]"<< std::endl;
	std::cout << "_connection = [" << parse_head.get_connection() << "]"<< std::endl;

 	if (valread != 0)
	{
		this->buff_send = meth.is_method(buff, src, this->port, parse_head);
		int nbr_bytes_send = 0;
		nbr_bytes_send = send(fd, buff_send.c_str(), buff_send.size(), 0);
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	} 
	close(fd);
}

/* void	Moteur::send_and_close(int fd, const std::vector<Server> & src)
{

	//buff_send = strdup("HTTP/1.1 400 Bad Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n<hr><center>nginx/1.14.0 (Ubuntu)</center>\n</body>\n</html>");
	//std::cout << "{{" << buff_send << "}}" << std::endl;

	int nbr_bytes_send = 0;
	nbr_bytes_send = send(fd, this->buff_send.c_str(), this->buff_send.size(), 0);
	if (nbr_bytes_send == -1)
		throw std::runtime_error("[Error] sent() failed");
	std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	close(fd);
}
 */
// loop server with EPOLLING events
void	Moteur::loop_server(const std::vector<Server> & src)
{
	int nbr_connexions = 0;
	int new_socket = 0;
	int i = 0;

	while (TRUE)
	{
		if ((nbr_connexions = epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, this->timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (i = 0; i < nbr_connexions; i++)
		{
			if (is_listener(this->fds_events[i].data.fd, this->listen_fd, this->nbr_servers, src))
			{
				new_socket = accept_connexions(this->fds_events[i].data.fd);
				fcntl(new_socket, F_SETFL, O_NONBLOCK);
				this->fds_events[i].events = EPOLLIN;
				this->fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, new_socket, &this->fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				read_send_data(this->fds_events[i].data.fd, src);
				//break ;
			}
		}
		//send_and_close(this->fds_events[i].data.fd, src);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
