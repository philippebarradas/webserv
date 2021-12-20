#ifndef LaunchServ_HPP
# define LaunchServ_HPP

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
#define PORT 12345
#define TRUE 1
#define FALSE 0
#define MAX_EVENTS 300

// My class
#include "../config/Server.hpp"

class Server;

class LaunchServ
{
	public:
		LaunchServ();
		LaunchServ(const Server & src);
		LaunchServ( LaunchServ const & src );
		~LaunchServ();
		void	setup_socket_server(const Server & src);
		void	loop_server(int listen_fd);
		LaunchServ &		operator=( LaunchServ const & rhs );
	private:
		struct epoll_event fds_events[MAX_EVENTS];
		int epfd;
		int	listen_fd;
		int	port;
		int timeout; // time before poll expiration
};

std::ostream &			operator<<( std::ostream & o, LaunchServ const & i );

#endif /* ********************************************************* MOTEUR_H */
