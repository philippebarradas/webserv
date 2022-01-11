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

// Creating socket file descriptor
int	LaunchServ::create_socket()
{
	int listen_fd = 0;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (listen_fd);
}

// Set socket file descriptor to be reusable
void	LaunchServ::set_socket(int listen_fd)
{
	int opt = 1;
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Put a name to a socket
void	LaunchServ::bind_socket(const std::vector<Server> & src, int listen_fd, size_t i)
{
	int port = 0;
	std::istringstream(src[i].getListen()) >> port;
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	std::cout << GREEN << "Port: " << port << std::endl << END;
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("[Error] Port already attribute");
}

// Make the socket passive, waiting to accept
void	LaunchServ::listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Accept connexion and return socket accepted
int	LaunchServ::accept_connexions(int listen_fd)
{
	int new_socket = 0;

	new_socket = accept(listen_fd, NULL, NULL);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	return (new_socket);
}

// Read data from buffer for now (after it will be the request send by client)
/* void	LaunchServ::read_data(int fd)
{
	int valread = 0;
	char buffer[100000];

	bzero(&buffer, sizeof(buffer));
	valread = recv(fd, buffer, sizeof(buffer), 0);
	if (valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	//if (valread == 0) // a voir quoi faire avec cette erreur
		//throw std::runtime_error("[Error] recv() finished");
}

// Send data to the client (telnet or browser)
void	LaunchServ::send_data(int fd)
{
	std::ifstream ifs;
	std::string	line, file;
	ifs.open("srcs/Server/to_delete.html", std::ifstream::in);
	while (std::getline(ifs, line))
	{
		file += line;
		file += '\n';
	}
	ifs.close();
	int nbr_bytes_send = 0;
	nbr_bytes_send = send(fd, file.c_str(), file.size(), 0);
	if (nbr_bytes_send == -1)
		throw std::runtime_error("[Error] sent() failed");
} */

// savoir si le fd dans le epoll est un listener (socket d'un port) ou non
bool	LaunchServ::is_listener(int fd, int *tab_fd, int nbr_servers)
{
	for (int i = 0; i < nbr_servers; i++)
	{
		if (fd == tab_fd[i])
			return (TRUE);
	}
	return (FALSE);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

LaunchServ::LaunchServ()
{
}

LaunchServ::LaunchServ(const std::vector<Server> & src)
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server(src);
	loop_server(src);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

LaunchServ::~LaunchServ()
{
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

LaunchServ&				LaunchServ::operator=( LaunchServ const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	LaunchServ::setup_socket_server(const std::vector<Server> & src)
{
	this->nbr_servers = src.size();
	this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	this->epfd = epoll_create(MAX_EVENTS);
	if (this->epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	//std::vector<Server>::iterator it = src.begin();
	for (this->i_server = 0; this->i_server < src.size(); this->i_server++)
	{
		this->listen_fd[this->i_server] = create_socket();
		this->fds_events[this->i_server].data.fd = this->listen_fd[this->i_server];
		this->fds_events[this->i_server].events = EPOLLIN;
		if (epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->listen_fd[this->i_server], &fds_events[this->i_server]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->listen_fd[this->i_server]);
		bind_socket(src, this->listen_fd[this->i_server], this->i_server);
		listen_socket(this->listen_fd[this->i_server]);
	}
}

#include "../method/method.hpp"
#include <stdlib.h>

int		buff_is_valid(char *buff)
{
	return (0);
}

void	read_send_data(int fd, const std::vector<Server> & src)
{
	int valread = 0;
	size_t buff_size = 1000;
	char buff[buff_size];

	std::string all_buff;
	std::string buff_send;

	
	Method meth;
	
	bzero(&buff, sizeof(buff));
	
	valread = recv(fd, buff, buff_size, 0);
	if (valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	std::cout << "((" << buff << "))" << std::endl;

	size_t x = 0;

/* 	while (buff_is_valid(buff) == 0 && x < 1)
	{
		valread = recv(fd, buff, buff_size, 0);
		if (valread == -1)
			throw std::runtime_error("[Error] recv() failed");
		
		std::cout << "((" << buff << "))" << std::endl;
		x++;
	} */

	
	buff_send = meth.is_method(buff, src);
	//buff_send = strdup("HTTP/1.1 400 Bad Request\nServer: localhost:12345/\nDate: Mon, 20 Dec 2021 14:10:48 GMT\nContent-Type: text/html\nContent-Length: 182\nConnection: close\n\n<html>\n<head><title>400 Bad Request</title></head>\n<body bgcolor='white'>\n<center><h1>400 Bad Request</h1></center>\n<hr><center>nginx/1.14.0 (Ubuntu)</center>\n</body>\n</html>");
	//std::cout << "{{" << buff_send << "}}" << std::endl;

	int nbr_bytes_send = 0;
	nbr_bytes_send = send(fd, buff_send.c_str(), buff_send.size(), 0);
	if (nbr_bytes_send == -1)
		throw std::runtime_error("[Error] sent() failed");
	std::cout << RED << "End of connexion" << END << std::endl << std::endl;
}


// loop server with EPOLLING events
void	LaunchServ::loop_server(const std::vector<Server> & src)
{
	int nbr_connexions = 0;
	int new_socket = 0;
	while (TRUE)
	{
		if ((nbr_connexions = epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, this->timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (int i = 0; i < nbr_connexions; i++)
		{
			if (is_listener(this->fds_events[i].data.fd, this->listen_fd, this->nbr_servers))
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
				//read_data(this->fds_events[i].data.fd);
				//send_data(this->fds_events[i].data.fd);
				close(this->fds_events[i].data.fd);
			}
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
