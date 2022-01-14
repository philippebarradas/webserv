/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:24:00 by user42            #+#    #+#             */
/*   Updated: 2022/01/14 16:37:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HEADER_HPP
# define PARSE_HEADER_HPP

#include "../Config/Server.hpp"

class Server;

class Parse_header
{
	public:
	    Parse_header();
		Parse_header( Parse_header const & src );
		~Parse_header();


		Parse_header &		operator=( Parse_header const & rhs );

        int		buff_is_valid(char *buff);
		int		parse_first_line(std::string buffer);


		std::vector<std::string> all_header;
		

		std::string get_method() const {return _method;};
		std::string get_path() const {return _path;};
		std::string get_protocol() const {return _protocol;};

		size_t	get_nbr_line() const {return this->_nbr_line;};
		size_t	get_request_status() const {return this->_request_status;};
		
		void	incr_nbr_line(){this->_nbr_line++;};
		void	set_nbr_line(size_t nbr){this->_nbr_line = nbr;};

	private:
		size_t	_request_status;

		size_t _nbr_line;

		std::string _method;
		std::string _path;
		std::string	_protocol;

		// request header
		
		std::string _host;
		std::string _user_agent;
		
		std::string _accept;
		std::string _accept_language;
		std::string _accept_encoding;
		std::string _method_charset;

		//general headers

		std::string _keep_alive;
		std::string _connection;
		std::string _upgrade_insecure_requests;

		// representation header

		std::string content_type;
		std::string	content_length;

		std::string _pragma;
		std::string _cache_control;



};


/////////////////////////////////////////////////////////

#endif


