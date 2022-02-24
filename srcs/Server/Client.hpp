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
		void	reinit_obj();

		// OPERATORS
		Client &		operator=( Client const & rhs );

		// SETTERS
		void	setRecv_len(int const &recv_len);
		void	setFill_request(char const & fill_request);
		void	setRequest_header_size(size_t const & rq_header_size);
		void	setFd(int const & fd);
		void	setIs_sendable(bool const & is_sendable);
		void	setHeader_parsed(bool const & header_parsed);

		// GETTERS
		int	& getRecv_len();
		size_t & getRequest_header_size();
		std::string & getFill_request();
		bool	& getHeader_parsed();
		bool	& getIs_sendable();
		int		& getFd();
		Parse_request & getParse_head();
		epoll_event & getEvents();


	private:

		// VARIABLES
		Client();
		Parse_request *_parse_head;
		epoll_event _events;
		int		_recv_len;
		size_t	_request_header_size;
		std::string _fill_request;
		bool	_header_parsed;
		bool	_is_sendable;
		int		_fd;
};

std::ostream &			operator<<( std::ostream & o, Client & i );

#endif /* ********************************************************* Client_H */
