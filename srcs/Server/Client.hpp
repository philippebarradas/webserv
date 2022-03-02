#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Engine.hpp"

class Server;
class Parse_request;

class Client
{
	public:

		// CONSTRUCTOR
		Client(epoll_event & ev);
		Client( Client const & src );

		// DESTRUCTOR
		~Client();

		// OPERATORS
		Client &		operator=( Client const & rhs );

		// SETTERS
		void	setRecv_len(size_t const &recv_len);
		void	setFill_request(char const & fill_request);
		void	setRequest_header_size(size_t const & rq_header_size);
		void	setFd(int const & fd);
		void	setIs_sendable(bool const & is_sendable);
		void	setHeader_parsed(bool const & header_parsed);
		void	setHeader_readed(bool const & header_readed);

		// GETTERS
		size_t const &		getRecv_len() const ;
		size_t const &		getRequest_header_size() const;
		std::string const &	getFill_request() const;
		bool const &		getHeader_parsed() const;
		bool const &		getHeader_readed() const;
		bool const &		getIs_sendable() const;
		int const &			getFd() const;
		Parse_request &		getParse_head();
		epoll_event &		getEvents();

	private:

		// CONSTRUCTOR
		Client();

		// VARIABLES
		std::vector<Parse_request>	_parse_head;
		epoll_event					_events;
		size_t						_recv_len;
		size_t						_request_header_size;
		std::string					_fill_request;
		bool						_header_parsed;
		bool						_header_readed;
		bool						_is_sendable;
};

#endif /* ********************************************************* Client_H */
