#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// C++
# include <iostream>
# include <string>

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

		Webserv &		operator=( Webserv const & rhs );

	private:
		int	fd_socket;

};

std::ostream &			operator<<( std::ostream & o, Webserv const & i );

#endif /* ********************************************************* WEBSERV_H */
