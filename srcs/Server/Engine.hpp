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
# include <sys/types.h>
# include <sys/wait.h>
# include <ext/stdio_filebuf.h>
# include <dirent.h>
# include <sys/stat.h>

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
# include "../Config/utils.hpp"
# include "../Parse_request/parse_request.hpp"
# include "Client.hpp"

// Colors
# define	PURPLE	"\033[0;35m"
# define	BLACK	"\033[1;30m"
# define	RED		"\033[1;31m"
# define	GREEN	"\033[1;32m"
# define	YELLOW	"\033[1;33m"
# define	BLUE	"\033[1;34m"
# define	PURPLE2	"\033[1;35m"
# define	CYAN	"\033[1;36m"
# define	WHITE	"\033[1;37m"
# define	END		"\033[0m"

// Utils macro
# define	MAX_EVENTS			2048
# define	MAX_SERVERS			100
# define	BUFFER_SIZE			1024
# define	BUFFER_SIZE_CHUNKED	16372


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
		std::map<int, Client>		_client;
		std::map<int, int>			_port_fd;				// (fd, port)  is _listen_fd
		//std::vector<int>			_port_already_set;		// is _port_binded
		struct	epoll_event			_ev;

		std::string					_length_chunk_string;
		size_t 						_length_chunk;
		size_t						_i_server_binded;
		size_t						_nbr_servers;
		int							_epfd;
		int							_port;
		int							_timeout;
		ssize_t						_valread;
		char						_buff[BUFFER_SIZE];
		char						_buff_chunked[BUFFER_SIZE_CHUNKED];
		std::string					_buff_send;
		std::string					_remote_port;
		std::string					_remote_addr;
		size_t						_fd_i;

		// METHODS
		void	delete_client(int const & fd_to_delete);
		void	init_fd_port(int const & port);
		int		create_socket();
		void	set_socket(const int & listen_fd);
		bool	is_binded(const int & port_config);
		void	bind_socket(const int & listen_fd, const std::vector<Server> & src);
		void	listen_socket(const int & listen_fd);
		int		accept_connexions(const int & listen_fd);
		void	accept_client(const int & to_accept);
		void	set_remote_var(struct sockaddr_in & addr_client);
		void	read_client(Client & client, int const & fd);
		void	mySend(const std::vector<Server> & src, Client & client);
		void	read_header(Client & client, int const & fd);
		void	read_body(Client & client, int const & fd);
		void	send_client(const std::vector<Server> & src, Client & client, int const & fd);
		bool	is_listener(const int & fd);
		bool	is_body_empty(Client & client);

		void 	read_content_length(Client & client, int const & fd);
		void	read_chunked(Client & client, int const & fd);


};

#endif /* ********************************************************* Engine_H */
