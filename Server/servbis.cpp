/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 18:58:44 by tsannie           #+#    #+#             */
/*   Updated: 2021/12/07 13:10:38by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "servbis.hpp"

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
	struct epoll_event	ev;
	struct epoll_event	events[200];
	int		one = 1;
	this->fd_listen	= -1;
	this->timeout	= 1;
	this->fd_socket	= -1;
	this->port = 5555;
	this->epfd = 0;
	//bzero(this->buffer, sizeof(this->buffer));

	this->fd_listen = socket(AF_INET, SOCK_STREAM, 0);
	this->fd_socket = setsockopt(this->fd_listen, SOL_SOCKET,  SO_REUSEADDR,
		&one, sizeof(int));
	this->fd_socket = fcntl(this->fd_listen, F_SETFL, O_NONBLOCK);
	this->my_bind(addr);
	this->fd_socket = listen(this->fd_listen, 32);
	this->epfd = epoll_create(1);
	std::cout << "Waiting on epoll()..." << std::endl;
	ev.data.fd = this->fd_listen;
	ev.events = EPOLLIN;
	this->timeout = (3 * 60 * 1000);
	loop_life_server(events, ev);
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

int	Webserv::my_bind(struct sockaddr_in addr)
{
	int ret = 0;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port        = htons(this->port);
	ret = bind(this->fd_listen, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0)
		throw std::runtime_error("[Error] bind() failed");
	return (ret);
}

/* void	Webserv::loop_epoll_fd_is_listening(struct epoll_event events[200], int end_server)
{
	this->epfd = 1;
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

		std::cout << "Listening socket is readable" << std::endl;
		//epoll_ctl(new_fd, EPOLL_CTL_ADD, 0, events);
		std::cout << "New incoming connection - " << new_fd << std::endl;
		events[this->nfds].data.fd = new_fd;
		events[this->nfds].events = EPOLLIN;
		this->nfds++;
	}
} */

int	Webserv::my_read(struct epoll_event events[200], int close_conn, int i)
{
	//bzero(this->buffer, sizeof(this->buffer));
	int nbr_bytes_read = 0;

	nbr_bytes_read = recv(events[i].data.fd, this->buffer, sizeof(this->buffer), 0);
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

int	Webserv::my_send(struct epoll_event events[200], int close_conn, int i,
			int nbr_bytes_read)
{
	std::cout << nbr_bytes_read << " bytes received" << std::endl;

	nbr_bytes_read = 10;
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
	int nbr_bytes_send = 0;
	nbr_bytes_send = send(events[i].data.fd, file.c_str(), file.size(), 0);

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

int	Webserv::loop_connection(struct epoll_event events[200], struct epoll_event ev, int close_conn)
{
	int new_fd = 0;
	int j = 0;
	int nbr_bytes_read = 0;
	this->nfds = 0;

	epoll_ctl(this->epfd, EPOLL_CTL_ADD, this->fd_listen, &ev);
	close_conn = 0;
	while (1)
	{
		this->nfds = epoll_wait(this->epfd, events, 200, this->timeout);
		for (j = 0; j < this->nfds; j++)
		{
			if (events[j].data.fd == this->fd_listen)
			{
				new_fd = accept(this->fd_listen, NULL, NULL);
				events[j].data.fd = new_fd;
				events[j].events = EPOLLIN;
				epoll_ctl(this->epfd, EPOLL_CTL_ADD, new_fd, &events[j]);
				std::cout << "nfds: " << this->nfds << std::endl;
				std::cout << "Descriptor num " << events[j].data.fd << " is readable" << std::endl;
			}
			else
			{
				nbr_bytes_read = my_read(events, close_conn, j);
				if (nbr_bytes_read == -1)
					break ;
				if (nbr_bytes_read == 0)
				{
					std::cout << "Descriptor num " << events[j].data.fd << " is deletable" << std::endl;
					epoll_ctl(this->epfd, EPOLL_CTL_DEL, events[j].data.fd, &ev);
					std::cout << "Connection closed" << std::endl;
					close_conn = 1;
					break ;
				}
				if (my_send(events, close_conn, j, nbr_bytes_read) == -1)
					break ;
			}
		}
	}
	return (j);
}

void	Webserv::loop_life_server(struct epoll_event events[200], struct epoll_event ev)
{
	int close_conn = 0;
	int i = 0;

	i = loop_connection(events, ev, close_conn); // send
	/* if (close_conn)
	{
		std::cout << "suce" << std::endl;
		close(events[i].data.fd);
		events[i].data.fd = -1;
		compress_array = 1;
	} */
	//ft_compress_array(compress_array, this->nfds, events, i);
	//ft_close(this->nfds, events);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
