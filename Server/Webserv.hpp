#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// C++
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

// Socket
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class Webserv
{

	public:
		Webserv();
		Webserv( Webserv const & src );
		~Webserv();
		int		my_bind(int listen_sd, struct sockaddr_in addr); // int ret in arg maybe
		void	loop_life_server(struct epoll_event events[200]);
		void	loop_epoll_fd_is_listening(struct epoll_event events[200], int end_server);
		void	loop_connection(struct epoll_event events[200], int close_conn, int i);
		int		my_read(struct epoll_event events[200], int close_conn, int i);
		int		my_send(struct epoll_event events[200], int close_conn, int i,
				int nbr_bytes_read);
		//int	add_port(); // later
		Webserv &		operator=( Webserv const & rhs );

	private:
		int	fd_socket;
		int	fd_listen;
		int	port;
		int timeout; // time before epoll expiration
		int nfds; // nbr fd
		char buffer[10000];
};

std::ostream &			operator<<( std::ostream & o, Webserv const & i );

#endif /* ********************************************************* WEBSERV_H */
