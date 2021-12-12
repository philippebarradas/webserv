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
//#include <sys/epoll.h> not on mac
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Colors
#define PURPLE	"\033[0;35m"
#define BLACK	"\033[1;30m"
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define PURPLE2 "\033[1;35m"
#define CYAN	"\033[1;36m"
#define WHITE	"\033[1;37m"
#define END	"\033[0m"

// Utils macro
#define PORT 8080
#define TRUE 1
#define FALSE 0
#define MAX_EVENTS 200

class Webserv
{
	public:
		Webserv();
		Webserv( Webserv const & src );
		~Webserv();
		void	setup_socket_server();
		void	loop_server(int listen_fd);
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
