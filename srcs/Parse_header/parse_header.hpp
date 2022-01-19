/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:24:00 by user42            #+#    #+#             */
/*   Updated: 2022/01/17 13:53:32 by user42           ###   ########.fr       */
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

		std::string get_request(std::string request) const
		{
			for (std::map<std::string, std::string>::const_iterator it = _big_tab.begin(); it != _big_tab.end(); ++it)
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

		int		buff_is_valid(char *buff, char *line);
		size_t	get_nbr_line() const {return this->_nbr_line;};
		void	incr_nbr_line(){this->_nbr_line++;};

	private:
		std::string	fill_big_tab(std::string str);
		int			parse_first_line();
		void		fill_variables();
		int			check_header();
		int			init_buffer(char *buff);


		std::map<std::string, std::string> _big_tab;
		std::string _buffer;
		size_t _nbr_line;
};

/////////////////////////////////////////////////////////

#endif
