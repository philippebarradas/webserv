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

std::string		sizet_to_string(size_t const & x);

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

		int		parse_request_buffer(std::string full_buffer);
		size_t	get_nbr_line() const {return this->_nbr_line;};
		void	incr_nbr_line(){this->_nbr_line++;};

		void	setStatus( std::string const & code ) { this->_header_tab["Status"] = code; }
		void	setTransfer( std::string const & method ) { this->_header_tab["Transfer-Encoding:"] = method; }

		std::map<std::string, std::string>	getBigMegaSuperTab( void ) const {return this->_header_tab;}
		std::map<std::string, std::string>	const & get_param_request_tab( void ) const {return this->_param_request_tab;}

		std::string get_request_body() const { return (_request_body); }
		size_t get_request_body_size() const { return (_request_body_size); };

		void	set_next_buffer_is_body(bool val){ _next_buffer_is_body = val; };
		void set_request_body(std::string new_request_body) { _request_body = new_request_body; }

		bool		_next_buffer_is_body;
		size_t		_request_body_size;
		std::string	_request_body;


	private:
		std::string	fill_header_tab(std::string str);
		int			fill_variables();
		void		fill_param_request_tab();

		int			parse_first_line();
		void		parse_path();
		void		is_body(size_t found);

		int			check_request();
		int			check_path();
		int			check_first_line(size_t full_size);
		int			check_double_content(std::map<std::string, std::string>::iterator replace);
		int			check_precondition();

       	std::map<std::string, std::string> _param_request_tab;
		std::map<std::string, std::string> _header_tab;

		std::string _buffer;
		size_t _nbr_line;
};

/////////////////////////////////////////////////////////

#endif

//  qeqwe465-/  :
