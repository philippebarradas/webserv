#ifndef MOTEUR_HPP
# define MOTEUR_HPP

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
//#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

class Webserv
{

	public:
		Webserv();
		Webserv( Webserv const & src );
		~Webserv();
		void	setup_socket_server();
		void	loop_server(int listen_fd);
		//int	add_port(); // later
		Webserv &		operator=( Webserv const & rhs );

	private:
		struct pollfd fds_events[200];
		int i_conn;
		int nbr_connexions;
		int	fd_socket;
		int	listen_fd;
		int	port;
		int timeout; // time before poll expiration
};

std::ostream &			operator<<( std::ostream & o, Webserv const & i );

#endif /* ********************************************************* WEBSERV_H */
