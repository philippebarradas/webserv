/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_request.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:24:00 by user42            #+#    #+#             */
/*   Updated: 2022/01/26 17:04:54 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Parse_request_HPP
# define Parse_request_HPP

#include "../Config/Server.hpp"

#define KEEP 0
#define STOP -1

std::string		int_to_string(size_t x);


class Server;

class Parse_request
{
	public:
	    Parse_request();
		Parse_request( Parse_request const & src );
		~Parse_request();
		Parse_request &		operator=( Parse_request const & rhs );

		std::string get_request(std::string request) const
		{
			for (std::map<std::string, std::string>::const_iterator it = _header_tab.begin(); it != _header_tab.end(); ++it)
			{
				if (request.compare(it->first) == 0)
				{
					if (it->second.compare("") != 0)
						return (it->second);
					return ("");
				}
			}
			return ("");
		};

		std::string get_request_body() const
		{
			return (_request_body);
		}

		void set_request_body(std::string new_request_body)
		{
			_request_body = new_request_body;
		}

		std::string get_request_body_size() const
		{
			return (int_to_string(_request_body_size));
		};

		void	set_next_buffer_is_body(bool val)
		{
			_next_buffer_is_body = val;
		};
		int		parse_request_buffer(char *buff, std::string full_b);
		size_t	get_nbr_line() const {return this->_nbr_line;};
		void	incr_nbr_line(){this->_nbr_line++;};
		void 	reinit_obj();

		std::map<std::string, std::string>	getBigMegaSuperTab( void ) const {return this->_header_tab;}

		bool		_next_buffer_is_body;
		size_t		_request_body_size;
		size_t		_client_max_body_size;
		std::string	_request_body;


/* 		bool _continue;
		size_t	recv_len;
		size_t	head; */
		std::string full_b;

	private:
		std::string	fill_header_tab(std::string str);
		int			parse_first_line();
		void		parse_path();
		int			fill_variables();
		int			init_buffer(char *buff);
		void		is_body(size_t found);

		int			check_request();
		int			check_first_line(size_t full_size);
		int			check_double_content(std::map<std::string, std::string>::iterator replace);
		int			check_precondition();


		std::vector<std::string> full_path;

		std::map<std::string, std::string> _header_tab;
		std::string _buffer;
		size_t _nbr_line;
};

/////////////////////////////////////////////////////////

#endif
