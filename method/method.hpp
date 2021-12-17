


#ifndef METHOD_HPP
# define METHOD_HPP
#define TRUE 1
#define FALSE 0
#include <iostream>
#include <cstring>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>

class Method
{
	public:
		Method(void); 								//can
		~Method(void);								//can
		Method(const Method &method);		//can
		Method &operator=(const Method &pt);	//can

		int is_method(std::string name_cmd, std::string method);

		void ft_get(std::string full_cmd);
		void ft_post(std::string full_cmd);
		void ft_delete(std::string full_cmd);

	private:


};


/////////////////////////////////////////////////////////

#endif



