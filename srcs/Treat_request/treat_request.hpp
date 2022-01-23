/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_request.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/20 14:31:39 by user42            #+#    #+#             */
/*   Updated: 2022/01/20 14:38:54 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef Treat_request_HPP
# define Treat_request_HPP
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
#include <sstream>


#include "../Config/Server.hpp"
#include "../Parse_header/parse_header.hpp"

std::string file_to_string(std::string file_path, std::string buff);

class Server;
class Parse_header;

class Treat_request
{
	public:
		Treat_request(void); 								//can
		~Treat_request(void);								//can
		Treat_request(const Treat_request &request);		//can
		Treat_request &operator=(const Treat_request &pt);	//can

		std::string is_Treat_request(std::string full_cmd, std::vector<Server> src, int port, const Parse_header & parse_head);
		
		std::string build_header(std::string buff);

		std::string is_bad_request(std::string buff);
		std::string is_not_allowed(std::string buff);
		std::string is_not_found(std::string buff);
		std::string	is_too_large(std::string buff);
		std::string	is_precondition_failed(std::string buff);

		std::string ft_get(std::string full_cmd, const Parse_header & parse_head);
		std::string ft_post(std::string full_cmd, const Parse_header & parse_head);
		std::string ft_delete(std::string full_cmd, const Parse_header & parse_head);


		std::string	get_date();



		std::vector<std::string> _header;
		std::string act_index;

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



