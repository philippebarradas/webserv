/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moteur.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/20 16:37:37 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "moteur.hpp"

// Creating socket file descriptor
int	create_socket()
{
	int listen_fd = 0;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (listen_fd);
}

// Set socket file descriptor to be reusable
void	set_socket(int listen_fd)
{
	int opt = 1;
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Put a name to a socket
void	bind_socket(int listen_fd)
{
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("[Error] bind_socket() failed");
}

// Make the socket passive, waiting to accept
void	listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Set first epoll_event with passive socket and first event to waiting read data
void	set_first_poll_events(struct epoll_event fds_events[MAX_EVENTS])
{
	for (int i = 0; i < MAX_EVENTS; i++) // init all fd to -1
		fds_events[i].data.fd = -1;
}

// Accept connexions, create new socket connected and put it at the end of events epoll_event
int	accept_connexions(int listen_fd, int nbr_connexions, struct epoll_event fds_events[MAX_EVENTS])
{
	int new_socket = 0;

	new_socket = accept(listen_fd, NULL, NULL);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	fds_events[nbr_connexions].data.fd = new_socket;
	fds_events[nbr_connexions].events = EPOLLIN;
	return (new_socket);
}

// Read data from buffer for now (after it will be the request send by client)


// Send data to the client (telnet or browser)
/* void	read_send_data(int i, struct epoll_event fds_events[MAX_EVENTS])
{
	int valread = 0;
	char buffer[100000];
	std::ifstream ifs;
	std::string	line, file;
	
	bzero(&buffer, sizeof(buffer));
	valread = recv(fds_events[i].data.fd, buffer, sizeof(buffer), 0);
	if (valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	if (valread == 0)
		throw std::runtime_error("[Error] recv() finished");
	std::cout << "||" << buffer << "||" << std::endl;
	ifs.open("to_delete.html", std::ifstream::in);
	while (std::getline(ifs, line))
	{
		file += line;
		file += '\n';
	}
	ifs.close();
	int nbr_bytes_send = 0;
	nbr_bytes_send = send(fds_events[i].data.fd, file.c_str(), file.size(), 0);
	if (nbr_bytes_send == -1)
		throw std::runtime_error("[Error] sent() failed");
	std::cout << RED << "End of connexion" << END << std::endl << std::endl;
} */

#include "../method/method.hpp"
#include <stdlib.h>

void	read_send_data(int i, struct epoll_event fds_events[MAX_EVENTS])
{
	int valread = 0;
	size_t buff_size = 1000000;
	char buff[buff_size];
	std::string buff_send;

	
	Method meth;
	
	bzero(&buff, sizeof(buff));
	valread = recv(fds_events[i].data.fd, buff, buff_size, 0);
	if (valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	if (valread == 0)
		throw std::runtime_error("[Error] recv() finished");
	std::cout << "((" << buff << "))" << std::endl;

	buff_send = meth.is_method(buff);
	//buff_send = strdup("HTTP/1.1 400 Bad Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n<hr><center>nginx/1.14.0 (Ubuntu)</center>\n</body>\n</html>");
	std::cout << "{{" << buff_send << "}}" << std::endl;

	int nbr_bytes_send = 0;
	nbr_bytes_send = send(fds_events[i].data.fd, buff_send.c_str(), buff_send.size(), 0);
	if (nbr_bytes_send == -1)
		throw std::runtime_error("[Error] sent() failed");
	std::cout << RED << "End of connexion" << END << std::endl << std::endl;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Webserv::Webserv()
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server();
	loop_server(this->listen_fd);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Webserv::~Webserv()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Webserv &				Webserv::operator=( Webserv const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Webserv::setup_socket_server()
{
	this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	this->listen_fd = create_socket();
	this->epfd = epoll_create(1);
	if (this->epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	fds_events[0].data.fd = this->listen_fd;
	fds_events[0].events = EPOLLIN;
	if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, listen_fd, &fds_events[0]) == -1)
		throw std::runtime_error("[Error] epoll_ctl_add() failed");
	set_socket(this->listen_fd);
	bind_socket(this->listen_fd);
	listen_socket(this->listen_fd);
	set_first_poll_events(this->fds_events);
}

// loop server with EPOLLING events
void	Webserv::loop_server(int listen_fd)
{
	int k = 0;
	int new_socket = 0;
	while (TRUE)
	{
		if ((k = epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, this->timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (int i = 0; this->fds_events[i].data.fd > 0; i++)
			std::cout << PURPLE2 << "fds_events[" << i << "] = " << this->fds_events[i].data.fd << std::endl;
		for (int i = 0; i < k; i++)
		{
			std::cout << YELLOW << "k = " << k << std::endl << END;
			if (this->fds_events[i].data.fd == listen_fd)
			{
				new_socket = accept_connexions(listen_fd, k, this->fds_events);
				fcntl(new_socket, F_SETFL, O_NONBLOCK);
				this->fds_events[i].events = EPOLLIN;
				this->fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, new_socket, &this->fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				read_send_data(i, fds_events);
				close(fds_events[i].data.fd);
			}
		}
	}
}


int	main( void )
{
	try
	{
		Webserv	serv;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
