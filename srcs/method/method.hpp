/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:39 by user42            #+#    #+#             */
/*   Updated: 2022/01/11 13:48:41 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

#include "../Config/Server.hpp"

std::string file_to_string(std::string file_path);

class Server;

class Method
{
	public:
		Method(void); 								//can
		~Method(void);								//can
		Method(const Method &method);		//can
		Method &operator=(const Method &pt);	//can

		std::string is_method(std::string full_cmd, std::vector<Server> src);
		
		std::string build_header(std::string buff);

		std::string is_bad_request(std::string buff);
		std::string is_not_allowed(std::string buff);

		std::string ft_get(std::string full_cmd);
		std::string ft_post(std::string full_cmd);
		std::string ft_delete(std::string full_cmd);


		std::string	get_date();



		std::vector<std::string> _header;

	private:

		std::string _request_status;
		std::string _server;
		std::string	_date;
		std::string	_content_type;
		std::string _content_length;
		std::string	_connection;
};


/////////////////////////////////////////////////////////

#endif



