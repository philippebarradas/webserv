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
#define END		"\033[0m"

// Utils macro
#define TRUE 1
#define FALSE 0
#define MAX_EVENTS 300
#define MAX_SERVERS 100

// My class
#include "../Config/Server.hpp"

class Server;

class Moteur
{
	public:
		Moteur();
		Moteur(const std::vector<Server> & src);
		Moteur( Moteur const & src );
		~Moteur();
		void	setup_socket_server(const std::vector<Server> & src);
		void	loop_server(const std::vector<Server> & src);
		Moteur &		operator=( Moteur const & rhs );
	private:
		struct epoll_event fds_events[MAX_EVENTS];
		size_t i_server;
		int nbr_servers;
		int epfd;
		int	listen_fd[MAX_SERVERS];
		int	port;
		int timeout; // time before poll expiration
		// private methods

		int		create_socket();
		void	set_socket(int listen_fd);
		void	bind_socket(int listen_fd, const std::vector<Server> & src);
		void	listen_socket(int listen_fd);
		int		accept_connexions(int listen_fd);
		void	read_send_data(int fd, const std::vector<Server> & src);
		bool	is_listener(int fd, int *tab_fd, int nbr_servers, const std::vector<Server> & src);
};

std::ostream &			operator<<( std::ostream & o, Moteur const & i );

#endif /* ********************************************************* MOTEUR_H */
