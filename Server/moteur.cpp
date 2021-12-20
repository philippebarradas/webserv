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
void	bind_socket(int listen_fd, std::vector<Server>::iterator it)
{
	int port = std::stoi((*it).getListen());
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	std::cout << GREEN << "Port: " << port << std::endl << END;
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		throw std::runtime_error("[Error] Port already attribute");
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
void	read_data(int i, struct epoll_event fds_events[MAX_EVENTS])
{
	int valread = 0;
	char buffer[100000];

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
	//std::cout << RED << "End of connexion" << END << std::endl << std::endl;
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

LaunchServ::LaunchServ()
{
	//std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	//setup_socket_server();
	//loop_server(this->listen_fd);
}

LaunchServ::LaunchServ(std::vector<Server> src, int nbr_servers)
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	for (int i = 0; i < nbr_servers; i++) // init all fd to -1
	{
		this->epfd[i] = -1;
		this->listen_fd[i] = -1;
	}
	setup_socket_server(src);
	loop_server();
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
void	LaunchServ::setup_socket_server(std::vector<Server> src)
{
	this->i_server = 0;
	this->nbr_servers = src.size();
	this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	std::vector<Server>::iterator it = src.begin();
	for (it = src.begin() ; it != src.end(); it++, this->i_server++)
	{
		this->listen_fd[this->i_server] = create_socket();
		this->epfd[this->i_server] = epoll_create(1);
		if (this->epfd[this->i_server] < 0)
			throw std::runtime_error("[Error] epoll_create() failed");
		fds_events[0].data.fd = this->listen_fd[this->i_server];
		fds_events[0].events = EPOLLIN;
		if (epoll_ctl(this->epfd[i_server], EPOLL_CTL_ADD, this->listen_fd[this->i_server], &fds_events[0]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->listen_fd[this->i_server]);
		bind_socket(this->listen_fd[this->i_server], it);
		listen_socket(this->listen_fd[this->i_server]);
	}
	set_first_poll_events(this->fds_events);
}

// loop server with EPOLLING events
void	LaunchServ::loop_server()
{
	int nbr_connexions = 0;
	int new_socket = 0;
	this->i_server = 0;
	while (TRUE)
	{
		if ((nbr_connexions = epoll_wait(this->epfd[this->i_server], this->fds_events, MAX_EVENTS, this->timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
	/* 	while (this->i_server < this->nbr_servers && nbr_connexions >= 0)
		{
			std::cout << PURPLE << "i_server in while: " << i_server << std::endl;
			if ((k = epoll_wait(this->epfd[this->i_server], this->fds_events, MAX_EVENTS, this->timeout)) < 0)
				std::cout << "[Error] epoll_wait() failed" << std::endl;
			else
				this->i_server++;
				//throw std::runtime_error("[Error] epoll_wait() failed");
		} */
		//for (int i = 0; this->fds_events[i].data.fd > 0; i++)
			//std::cout << PURPLE2 << "fds_events[" << i << "] = " << this->fds_events[i].data.fd << std::endl;
		for (int i = 0; i < nbr_connexions; i++)
		{
			//std::cout << YELLOW << "k = " << k << std::endl << END;
			if (this->fds_events[i].data.fd == this->listen_fd[this->i_server])
			{
				new_socket = accept_connexions(this->listen_fd[this->i_server], nbr_connexions, this->fds_events);
				fcntl(new_socket, F_SETFL, O_NONBLOCK);
				this->fds_events[i].events = EPOLLIN;
				this->fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->epfd[this->i_server], EPOLL_CTL_ADD, new_socket, &this->fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				read_data(i, fds_events);
				send_data(i, fds_events);
				close(fds_events[i].data.fd);
			}
		}
		//if (this->i_server == this->nbr_servers)
			//break ;
		//this->i_server++;
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
