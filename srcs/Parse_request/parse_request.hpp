#ifndef Parse_request_HPP
# define Parse_request_HPP

#include "../Config/Server.hpp"

std::string		sizet_to_string(size_t const & x);

class Server;

class Parse_request
{
	public:
		Parse_request();
		Parse_request( Parse_request const & src );
		~Parse_request();
		Parse_request &		operator=( Parse_request const & rhs );


		// GET

		std::string get_request(std::string request) const;;
		size_t	get_nbr_line() const;

		std::map<std::string, std::string>	getBigMegaSuperTab( void ) const;
		std::map<std::string, std::string>	const & get_param_request_tab( void ) const;

		std::string get_request_body() const;
		size_t get_request_body_size() const;


		// SET

		void	set_request_body(std::string new_request_body);
		void	setStatus( std::string const & code );
		void	setConnection( std::string const & status);
		void	setTransfer( std::string const & method );
		void	incr_nbr_line();

		//

		int		parse_request(std::string full_buffer);
		int		parse_first_line(std::string full_buffer);
		int		parse_body(std::string full_buffer);

		bool first_line_is_parsed;
		bool error_first_line;

	private:

		std::string	_buffer;
		size_t		_nbr_line;
	// FIRST LINE
		int			fill_first_line();
		void		parse_path();
		int			check_first_line(size_t full_size);
		int			check_path();
	// REQUEST
		int			fill_variables();
		std::string	fill_header_tab(std::string str);
		void		fill_param_request_tab(std::string buff_parsed);

		int			check_request();
		int			check_double_content();
		int			check_precondition();

       	std::map<std::string, std::string> _param_request_tab;
		std::map<std::string, std::string> _header_tab;
	// BODY
		void		is_body(size_t found);

		std::string	_request_body;
		size_t		_request_body_size;
	//
};

/////////////////////////////////////////////////////////

#endif
