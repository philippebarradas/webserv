/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 18:58:44 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/08 18:08:36 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

void	ft_compress_array(int compress_array, int nfds, struct epoll_event events[200], int i)
{
	if (compress_array)
	{
		compress_array = 0;
		for (i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == -1)
			{
				for (int j = i; j < nfds; j++)
					events[j].data.fd = events[j + 1].data.fd;
				i--;
				nfds--;
			}
		}
	}
}

void	ft_close(int nfds, struct epoll_event events[200])
{
	for (int i = 0; i < nfds; i++)
	{
		if (events[i].data.fd >= 0)
			close(events[i].data.fd);
	}
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Webserv::Webserv()
{
	struct sockaddr_in	addr;
	struct epoll_event	events[200];
	int		one = 1;
	this->fd_listen	= -1;
	this->timeout	= 1;
	this->fd_socket	= -1;
	this->port = 5555;
	bzero(this->buffer, sizeof(this->buffer));

	this->fd_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd_listen < 0)
		throw std::runtime_error("[Error] socket() failed" + std::string(strerror(errno)));

	this->fd_socket = setsockopt(fd_listen, SOL_SOCKET,  SO_REUSEADDR,
		&one, sizeof(int));
	if (this->fd_socket < 0)
		throw std::runtime_error("[Error] setsockopt() failed");

	this->fd_socket = fcntl(fd_listen, F_SETFL, O_NONBLOCK);
	if (this->fd_socket < 0)
		throw std::runtime_error("[Error] fcntl() failed");
	this->my_bind(this->fd_listen, addr);
	fd_socket = listen(this->fd_listen, 32);
	if (fd_socket < 0)
		throw std::runtime_error("[Error] listen() failed");
	events[0].data.fd = this->fd_listen;
	events[0].events = EPOLLIN;
	this->timeout = (3 * 60 * 1000);
	loop_life_server(events);
}

Webserv::Webserv( const Webserv & src )
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Webserv::~Webserv()
{
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

int	Webserv::my_bind(int listen_fd, struct sockaddr_in addr)
{
	int ret = 0;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port        = htons(this->port);
	ret = bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
		throw std::runtime_error("[Error] bind() failed");
	return (ret);
}

void	Webserv::loop_epoll_fd_is_listening(struct epoll_event events[200], int end_server)
{
	int epfd = 1;
	int new_fd = 1;
	while (new_fd != -1)
	{
		new_fd = accept(this->fd_listen, NULL, NULL);
		if (new_fd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				throw std::runtime_error("[Error] accept() failed");
				end_server = 1;
			}
			break;
		}
		epfd = epoll_create(1);
		std::cout << "Waiting on epoll()..." << std::endl;
		if (epfd < 0)
		{
			throw std::runtime_error("[Error] epoll_create() failed");
			break;
		}
		if (epfd == 0)
		{
			std::cout << "Epoll_create() timed out.  End program." << std::endl;
			break;
		}
		std::cout << "Listening socket is readable" << std::endl;
		//epoll_ctl(new_fd, EPOLL_CTL_ADD, 0, events);
		epoll_ctl(epfd, EPOLL_CTL_ADD, new_fd, &events[this->nfds - 1]);
		std::cout << "New incoming connection - " << new_fd << std::endl;
		events[this->nfds].data.fd = new_fd;
		events[this->nfds].events = EPOLLIN;
		this->nfds++;
	}
}

int	Webserv::my_read(struct epoll_event events[200], int close_conn, int j)
{
	int nbr_bytes_read = 0;


	std::cout << "je suis dans read" << std::endl;
	nbr_bytes_read = recv(events[j].data.fd, this->buffer, sizeof(this->buffer), 0);
	if (nbr_bytes_read < 0)
	{
		if (errno != EWOULDBLOCK)
		{
			throw std::runtime_error("[Error] recv() failed");
			close_conn = 1;
		}
		return (-1);
	}
	return (nbr_bytes_read);
}

int	Webserv::my_send(struct epoll_event events[200], int close_conn, int j,
			int nbr_bytes_read)
{
	std::cout << nbr_bytes_read << " bytes received" << std::endl;
	//std::cout << "this->buffer: |" << this->buffer << "|" << std::endl;
	std::ifstream ifs;

	ifs.open("to_delete.html", std::ifstream::in);
	std::string	line;
	std::string	file;
	while (std::getline(ifs, line))
	{
		file += line;
		file += '\n';
	}
	ifs.close();
	//std::cout << file << std::endl;
	int nbr_bytes_send = 0;
	nbr_bytes_send = send(events[j].data.fd, file.c_str(), file.size(), 0);

	//nbr_bytes_send = send(events[i].data.fd, this->buffer, nbr_bytes_read, 0);
	if (nbr_bytes_send < 0)
	{
		throw std::runtime_error("[Error] send() failed");
		close_conn = 1;
		return (-1);
	}
	//std::cout << "this->buffer: |" << this->buffer << "|" << std::endl;
	return (0);
}

void	Webserv::loop_connection(struct epoll_event events[200], int close_conn, int i)
{
	int k = 0;
	int nbr_bytes_read = 0;
	//bzero(&this->buffer, sizeof(this->buffer));
	//k = epoll_wait(this->fd_socket, events, 200, this->timeout);
	//std::cout << "k: " << k << std::endl;
	//std::cout << "Descriptor num " << events[i].data.fd << " is readable" << std::endl;
	close_conn = 0;
	while (1)
	{
		k = epoll_wait(this->fd_socket, events, 200, this->timeout);
		std::cout << "k: " << k << std::endl;
		std::cout << "Descriptor num " << events[i].data.fd << " is readable" << std::endl;
	}
}

void	Webserv::loop_life_server(struct epoll_event events[200])
{
	int close_conn = 0, compress_array = 0, end_server = 0;
	while (!end_server)
	{
		this->nfds = 1;
		for (int i = 0; i < this->nfds; i++)
		{
			if (events[i].events == 0)
				continue;
			if (events[i].events != EPOLLIN)
			{
				std::cout << "Error! events = " << events[i].events << std::endl;
				end_server = 1;
				break ;
			}
			if (events[i].data.fd == this->fd_listen)
				loop_epoll_fd_is_listening(events, end_server); // read
			else
				loop_connection(events, close_conn, i); // send
			if (close_conn)
			{
				std::cout << "suce" << std::endl;
				close(events[i].data.fd);
				events[i].data.fd = -1;
				compress_array = 1;
			}
			//ft_compress_array(compress_array, this->nfds, events, i);
		}
	}
	ft_close(this->nfds, events);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
