/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moteur.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/14 23:06:40 by dodjian          ###   ########.fr       */
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
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	int opt = 1;
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
	if (listen(listen_fd, 3) < 0)
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
void	read_data(int i, struct epoll_event fds_events[MAX_EVENTS])
{
	int valread = 0;
	char buffer[10000];

	bzero(&buffer, sizeof(buffer));
	valread = recv(fds_events[i].data.fd, buffer, sizeof(buffer), 0);
	if (valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	if (valread == 0)
		throw std::runtime_error("[Error] recv() finished");
}

// Send data to the client (telnet or browser)
void	send_data(int i, struct epoll_event fds_events[MAX_EVENTS])
{
	std::ifstream ifs;
	std::string	line, file;

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
	//this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	int k = 0;
	int new_socket = 0;
	while (TRUE)
	{
		if ((k = epoll_wait(this->epfd, this->fds_events, MAX_EVENTS, -1)) < 0)
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
				read_data(i, fds_events);
				send_data(i, fds_events);
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
