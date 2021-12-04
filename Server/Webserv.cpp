/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 18:58:44 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/05 00:51:48 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"


void	Webserv::loop_epoll_fd_is_listening(struct epoll_event events[200], int end_server, int nfds)
{
	int new_fd = 1;
	while (!new_fd)
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
		this->fd_socket = epoll_create(1);
		std::cout << "Waiting on poll()..." << std::endl;
		if (this->fd_socket < 0)
		{
			throw std::runtime_error("[Error] epoll_create() failed");
			break;
		}
		if (this->fd_socket == 0)
		{
			std::cout << "Epoll_create() timed out.  End program." << std::endl;
			break;
		}
		std::cout << "Listening socket is readable" << std::endl;
		epoll_ctl(new_fd, EPOLL_CTL_ADD, 0, events);
		std::cout << "New incoming connection - %d" << new_fd << std::endl;
		events[nfds].data.fd = new_fd;
		events[nfds].events = EPOLLIN;
		nfds++;
	}
}

int	Webserv::my_read(struct epoll_event events[200], int close_conn, int i)
{
	int nbr_bytes_read = 0;
	char buffer[1000];

	bzero(buffer, sizeof(buffer));
	nbr_bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
	std::cout << "nbr bytes read: %d" << nbr_bytes_read << std::endl;
	if (nbr_bytes_read < 0) // nbr of bytes read
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

int	Webserv::my_send(struct epoll_event events[200], int close_conn, int i)
{

}

void	Webserv::loop_connection(struct epoll_event events[200], int close_conn, int i)
{
	int nbr_bytes_read = 0;

	epoll_wait(events[i].data.fd, events, 200, this->timeout);
	std::cout << "Descriptor is readable - %d" << events[i].data.fd << std::endl;
	close_conn = 0;
	while (1)
	{
		nbr_bytes_read = my_read(events, i, close_conn);
		if (nbr_bytes_read == -1)
			break ;
		if (nbr_bytes_read == 0)
		{
			std::cout << "Connection closed" << std::endl;
			close_conn = 1;
			break;
		}
		rc = send(events[i].data.fd, buffer, nbr_bytes_read, 0);
	}
}

void	Webserv::loop_life_server(struct epoll_event events[200])
{
	int close_conn, compress_array = 0;
	int	end_server = 0;
	while (!end_server)
	{
		int nfds = 1;
		for (int i = 0; i < nfds; i++)
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
				loop_epoll_fd_is_listening(events, end_server, nfds);
			else
				loop_connection(events, close_conn, i);
			if (close_conn)
			{
				close(events[i].data.fd);
				events[i].data.fd = -1;
				compress_array = 1;
			}
		}
	}
}

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
	this->port = 12345;

	fd_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_listen < 0)
		throw std::runtime_error("[Error] socket() failed" + std::string(strerror(errno)));

	this->fd_socket = setsockopt(fd_listen, SOL_SOCKET,  SO_REUSEADDR,
		&one, sizeof(int));
	if (this->fd_socket < 0)
		throw std::runtime_error("[Error] setsockopt() failed");

	this->fd_socket = fcntl(fd_listen, F_SETFL, O_NONBLOCK);
	if (this->fd_socket < 0)
		throw std::runtime_error("[Error] fcntl() failed");
	this->my_bind(fd_listen, addr);
	fd_socket = listen(fd_listen, 32);
	if (fd_socket < 0)
		throw std::runtime_error("[Error] listen() failed");
	events[0].data.fd = fd_listen;
	events[0].events = EPOLLIN;
	timeout = (3 * 60 * 1000);
	loop_life_server(events);
}

Webserv::Webserv( const Webserv & src )
{
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Webserv const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

void	listen(  )


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
