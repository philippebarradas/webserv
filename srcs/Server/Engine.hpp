#ifndef ENGINE_HPP
# define ENGINE_HPP

// C++
# include <iostream>
# include <string>
# include <fstream>
# include <cstring>

// C
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>

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

// Utils macro
#define MAX_EVENTS 300
#define MAX_SERVERS 100
#define BUFFER_SIZE 1024

// My class
#include "../Config/Server.hpp"
#include "../Parse_request/parse_request.hpp"
#include "Client.hpp"

class Server;
class Parse_request;
class Client;

class Engine
{
	public:

		// CONSTRUCTOR
		Engine();
		Engine(const std::vector<Server> & src);
		Engine(const Engine & src);

		// DESTRUCTOR
		~Engine();

		// METHODS
		void	setup_socket_server(const std::vector<Server> & src);
		void	loop_server(const std::vector<Server> & src);

		// OPERATORS
		Engine &		operator=(const Engine & rhs);

		// GETTERS
		std::string	const & GetRemote_Port() const;
		std::string	const & GetRemote_Addr() const;
		int			const & GetAccessPort(void) const;

	private:

		// VARIABLES
		struct	sockaddr_in	_addr;
		struct	epoll_event	_fds_events[MAX_EVENTS];
		std::vector<Client>	_v;
		size_t				_i_server;
		size_t				_i_server_binded;
		size_t				_nbr_servers;
		int					_epfd;
		int					_listen_fd[MAX_SERVERS];
		int					_port_binded[MAX_SERVERS];
		int					_port;
		int					_timeout; // time before poll expiration
		ssize_t				_valread;
		char				_buff[BUFFER_SIZE];
		std::string			_buff_send;
		std::string			_remote_port;
		std::string			_remote_addr;

		// METHODS
		int		create_socket();
		void	set_socket(const int & listen_fd);
		bool	is_binded(const int & port_config);
		void	bind_socket(const int & listen_fd, const std::vector<Server> & src);
		void	listen_socket(const int & listen_fd);
		int		accept_connexions(const int & listen_fd);
		void	loop_accept(const int & nbr_connexions, const std::vector<Server> & src);
		void	loop_input_output(const std::vector<Server> & src);
		void	set_remote_var(struct sockaddr_in & addr_client);
		void	myRead(Client & client);
		void	mySend(const std::vector<Server> & src, Client & client);
		void	read_header(Client & client);
		void	read_body(Client & client);
		void	send_data(const std::vector<Server> & src, Client & client);
		bool	is_listener(const int & fd, const int *tab_fd,
			const int & nbr_servers, const std::vector<Server> & src);
		bool	is_body_empty(Client & client);
};

#endif /* ********************************************************* Engine_H */
