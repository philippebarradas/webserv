#ifndef CLIENT_HPP
# define CLIENT_HPP

// C++
# include <iostream>
# include <string>
# include <fstream>
# include <cstring>

// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// Socket
# include <sys/ioctl.h>
# include <sys/poll.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <netinet/in.h>

// My class
# include "../Config/Server.hpp"
# include "../Response/TreatRequest.hpp"

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

// My class
#include "../Config/Server.hpp"
#include "../Parse_request/parse_request.hpp"

class Server;
class Parse_request;

class Client
{
	public:

		// CONSTRUCTOR
		Client(epoll_event & ev);

		Client( Client const & src );

		// DESTRUCTOR
		~Client();

		// METHODS


		// OPERATORS
		Client &		operator=( Client const & rhs );

		// SETTERS


		// GETTERS

		Parse_request & getParse_head();
		epoll_event & getEvents();

		//bool _is_create

		void	reinit_obj();

		size_t	recv_len;
		size_t	request_header_size;
		std::string fill_request;
		bool	is_parsed;
		bool	is_sendable;

		int		fd;

	private:

		Client();
		Parse_request *_parse_head;
		epoll_event _events;
		// VARIABLES
	/* 	struct	sockaddr_in _addr;
		struct	epoll_event _fds_events[MAX_EVENTS];
		size_t	_i_server;
		size_t	_i_server_binded;
		size_t	_nbr_servers;
		int	_epfd;
		int	_listen_fd[MAX_SERVERS];
		int	_port_binded[MAX_SERVERS];
		int	_port;
		int	_timeout; // time before poll expiration
		int	_valread;
		char	_buff[BUFFER_SIZE];
		std::string	_buff_send;
		std::string	_remote_port;
		std::string	_remote_addr;

		// METHODS
		int		create_socket();
		void	set_socket(int listen_fd);
		bool	is_binded(int port_config);
		void	bind_socket(int listen_fd, const std::vector<Server> & src);
		void	listen_socket(int listen_fd);
		int		accept_Clients(int listen_fd);
		void	set_remote_var(struct sockaddr_in & addr_Client);
		void	read_send_data(int fd, const std::vector<Server> & src);
		void	send_data(int valread, int fd,const std::vector<Server> & src, const Parse_request & parse_head);

		bool	is_listener(int fd, int *tab_fd, int nbr_servers, const std::vector<Server> & src); */
};

std::ostream &			operator<<( std::ostream & o, Client & i );

#endif /* ********************************************************* Client_H */