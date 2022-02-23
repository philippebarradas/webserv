#ifndef Connexion_HPP
# define Connexion_HPP

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

// Utils macro
#define TRUE 1
#define FALSE 0
#define MAX_EVENTS 300
#define MAX_SERVERS 100

// My class
#include "../Config/Server.hpp"
#include "../Parse_request/parse_request.hpp"

class Server;
class Parse_request;

class Connexion
{
	public:

		// CONSTRUCTOR
		Connexion();

		Connexion( Connexion const & src );

		// DESTRUCTOR
		~Connexion();

		// METHODS


		// OPERATORS
		Connexion &		operator=( Connexion const & rhs );

		// GETTERS

        //bool _is_create
		
		size_t	recv_len;
		size_t	request_header_size;
		std::string fill_request;
		bool	is_parsed;
		bool	is_sendable;
		char b;
		
	private:

};

std::ostream &			operator<<( std::ostream & o, Connexion const & i );

#endif /* ********************************************************* Connexion_H */
