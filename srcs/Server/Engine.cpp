/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2022/03/03 23:54:06 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

void signal_to_exit( int ssignum )
{
	std::cout << "SIGNALLLLLLLLLLLLLLLLLLLLLLLLL" << std::endl;
	static_cast<void>(ssignum);
	throw SignalStop();
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Engine::Engine()
{
}

Engine::Engine(const std::vector<Server> & src)
{
	std::cout << BBLUE "-------- Starting webserv --------\n" END << std::endl;
	try
	{
		signal(SIGINT, signal_to_exit);
		/*bzero(events_fd, sizeof(_fds_events));
		for (size_t i = 0 ; i < MAX_EVENTS ; ++i)
			_fds_events[i].data.fd = -1;
		for (size_t i = 0 ; i < MAX_EVENTS ; ++i)
			std::cout << _fds_events[i].data.fd << std::endl;*/
		setup_socket_server(src);
		loop_server(src);
	}
	catch( SignalStop const & e )
	{
		static_cast<void>(e);
	}
	std::cout << "--------------------------- LEAVE ---------------------------" << std::endl;

}

Engine::Engine(const Engine & src)
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Engine::~Engine()
{
	std::map<int, Client>::iterator		pc_it, pc_end;
	std::map<int, int>::iterator		ps_it, ps_end;

	pc_end = _client.end();
	for (pc_it = _client.begin(); pc_it != pc_end ; ++pc_it)
		close(pc_it->first);

	ps_end = _port_fd.end();
	for (ps_it = _port_fd.begin(); ps_it != ps_end ; ++ps_it)
		close(ps_it->first);

	close(_epfd);
	std::cout << BBLUE "\n--------- End of webserv ---------" END << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Engine&				Engine::operator=(const Engine & rhs)
{
	if (this != &rhs)
	{
		this->_buff_send = rhs._buff_send;
		this->_epfd = rhs._epfd;
		this->_nbr_servers = rhs._nbr_servers;
		this->_port = rhs._port;
		this->_remote_addr = rhs._remote_addr;
		this->_remote_port = rhs._remote_port;
		this->_timeout = rhs._timeout;
		this->_valread = rhs._valread;
		this->_client = rhs._client;
		this->_port_fd = rhs._port_fd;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/


void	Engine::delete_client(int const & fd_to_delete)
{
	this->_client.erase(fd_to_delete);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, fd_to_delete, NULL);
	close(fd_to_delete);
}

int	Engine::create_socket()
{
	int	listen_fd = 0;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (listen_fd);
}

void	Engine::set_socket(const int & listen_fd)
{
	int	opt = 1;

	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
}

bool	Engine::is_binded(const int & port_config)
{
	std::map<int, int>::const_iterator	it, end;

	end = this->_port_fd.end();
	for (it = this->_port_fd.begin() ; it != end ; ++it)
	{
		if (port_config == it->second)
			return (true);
	}
	return (false);
}

void	Engine::listen_socket(const int & listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

void	Engine::set_remote_var(struct sockaddr_in & addr_client)
{
	int	i_remote_port = ntohs(addr_client.sin_port);

	std::stringstream ss;
	ss << i_remote_port;
	this->_remote_port = ss.str();
	this->_remote_addr = inet_ntoa(addr_client.sin_addr);
}

int	Engine::accept_connexions(const int & listen_fd)
{
	struct sockaddr_in	addr_client;
	int	new_socket;
	int	client_len = sizeof(addr_client);

	new_socket = accept(listen_fd, (struct sockaddr *)&addr_client,
		(socklen_t *)&client_len);
	if (fcntl(new_socket, F_SETFL, O_NONBLOCK))
		throw std::runtime_error("[Error] fcntl() failed");
	std::cout << PURPLE2 << "[webserv] new client accept on: " << this->_port
		<< std::endl << END;
	set_remote_var(addr_client);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");

	return (new_socket);
}

bool	Engine::is_listener(const int & fd)
{
	std::map<int, int>::const_iterator	it, end;

	end = this->_port_fd.end();
	for (it = this->_port_fd.begin() ; it != end ; ++it)
	{
		if (fd == it->first)
		{
			this->_port = it->second;
			return (true);
		}
	}
	return (false);
}

bool	Engine::is_body_empty(Client & client)
{
	if (client.getParse_head().get_request("Content-Length:") != ""
	|| client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
		return (false);
	return (true);
}

void	Engine::init_fd_port(int const & port)
{
	struct sockaddr_in	s_port;
	int	fd;

	fd = create_socket();
	set_socket(fd);
	bzero(&s_port, sizeof(s_port));
	s_port.sin_family = AF_INET;
	s_port.sin_addr.s_addr = INADDR_ANY;
	s_port.sin_port = htons(port);
	std::cout << PURPLE2 << "[webserv] listen on: " << port
		<< std::endl << END;
	if (bind(fd, (struct sockaddr *)&s_port,
		sizeof(s_port)) < 0)
			throw std::runtime_error("[Error] Bind failed");
	listen_socket(fd);

	std::cout << "INSERT" << std::endl;
	this->_port_fd.insert(std::make_pair(fd, port));
}

void	Engine::setup_socket_server(const std::vector<Server> & src)
{
	std::map<int, int>::const_iterator	it, end;
	int		port_config;
	size_t	i;

	this->_port = 0, this->_valread = -1;
	this->_timeout = 3 * 60 * 1000;
	this->_epfd = epoll_create(MAX_EVENTS);

	if (this->_epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	for (i = 0 ; i < src.size() ; ++i)
	{
		port_config = 0;
		std::istringstream(src[i].getListen()) >> port_config;
		std::cout << "port_config\t=\t" << port_config << std::endl;
		if (!is_binded(port_config))
			init_fd_port(port_config);
	}
	bzero(&this->_ev, sizeof(this->_ev));
	end = this->_port_fd.end();
	for (it = this->_port_fd.begin() ; it != end ; ++it)
	{
		this->_ev.data.fd = it->first;
		this->_ev.events = EPOLLIN;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD,
			it->first, &this->_ev) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
	}
	std::cout << std::endl;
}

void	Engine::read_header(Client & client, int const & fd)
{
	char	b;

	_valread = recv(fd, &b, 1, 0);
	if (_valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	else if (!((b == '\n' || b == '\r')
		&& client.getFill_request().size() == 0))
	{
		client.setRecv_len(_valread);
		client.setFill_request(b);
	}
	if (client.getFill_request().find("\r\n\r\n") != std::string::npos)
	{
		client.setHeader_readed(true);
		client.setRequest_header_size(client.getFill_request().size());
	}
	else if (client.getFill_request().find("\r\n") != std::string::npos &&
		client.getParse_head().first_line_is_parsed == false)
	{
		client.getParse_head().parse_first_line(client.getFill_request());
		client.getParse_head().first_line_is_parsed = true;
 		if (client.getParse_head().get_request("Status") != "200")
			client.getParse_head().error_first_line = true;
	}

}

void Engine::read_content_length(Client & client, int const & fd)
{
	size_t length_body =  stost_size(0, MAX_MAXBODY,
		client.getParse_head().get_request("Content-Length:"), "_request_body_size");
	size_t length_request = (client.getRequest_header_size() + length_body);
	if (client.getFill_request().size() < length_request)
	{
		if (client.getFill_request().size() + BUFFER_SIZE > length_request)
			_valread = recv(fd, &_buff,
			length_request - (client.getFill_request().size()), 0);
		else
			_valread = recv(fd, &_buff, BUFFER_SIZE, 0);
		client.setRecv_len(_valread);
		client.setFill_request_body(_buff, _valread);
	}
	if (client.getFill_request().size() >= (client.getRequest_header_size() +
		stost_size(0, MAX_MAXBODY,
		client.getParse_head().get_request("Content-Length:"), "_request_body_size")))
	{
		client.getParse_head().parse_body(client.getFill_request());
		client.setIs_sendable(true);
	}
}

void Engine::read_chunked(Client & client, int const & fd)
{
	char b;

	if (client.getFill_request().find("0\r\n\r\n") == std::string::npos)
	{
		if (_length_chunk_string.find("\r\n") != std::string::npos)
		{
			_length_chunk = hexa_to_size_brut(_length_chunk_string);
			_length_chunk_string = "";
		}
		if (_length_chunk == 0)
		{
			_valread = recv(fd, &b, 1, 0);
			_length_chunk_string += b;
			client.setRecv_len(_valread);
			client.setFill_request(b);
		}
		else
		{
			_valread = recv(fd, &_buff_chunked, _length_chunk, 0);
			client.setRecv_len(_valread);
			client.setFill_request_body(_buff_chunked, _valread);
			_length_chunk = 0;
		}
	}
	if (client.getFill_request().find("0\r\n\r\n") != std::string::npos)
	{
		client.getParse_head().parse_body(client.getFill_request());
		client.setIs_sendable(true);
	}
}

void	Engine::read_body(Client & client, int const & fd)
{
	bzero(_buff, BUFFER_SIZE);
	bzero(_buff_chunked, BUFFER_SIZE_CHUNKED);
	if (client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
		read_chunked(client, fd);
	else
		read_content_length(client, fd);
}

void	Engine::send_client(const std::vector<Server> & src, Client & client, int const & fd)
{
	int		nbr_bytes_send = 0;

	if (_valread != 0)
	{
		TreatRequest	treatment(src, *this);
		this->_buff_send = treatment.treat(client.getParse_head());
		nbr_bytes_send = send(fd,
			this->_buff_send.c_str(), this->_buff_send.size(), 0);
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		this->_ev.events = EPOLLIN;
		this->_ev.data.fd = fd;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, &this->_ev) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
	}
}

void	Engine::accept_client(const int & to_accept)
{
	int	new_socket;

	new_socket = accept_connexions(to_accept);
	this->_ev.data.fd = new_socket;
	this->_ev.events = EPOLLIN;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket,
		&this->_ev) == -1)
		throw std::runtime_error("[Error] epoll_ctl_add() failed");
	//std::cout << "CLIENT ACCEPT" << std::endl;
	this->_client.insert(std::make_pair(this->_ev.data.fd, Client()));
}

void	Engine::read_client(Client & client, int const & fd)
{
	if (client.getHeader_readed() == false)
		read_header(client, fd);
	if (client.getHeader_parsed() == false && client.getHeader_readed() == true)
	{
		client.getParse_head().parse_request(client.getFill_request());
		client.setHeader_parsed(true);
	}
	else if (is_body_empty(client) == false)
		read_body(client, fd);
	if (client.getIs_sendable() == true
		|| client.getParse_head().error_first_line == true
		|| (client.getHeader_parsed() == true && is_body_empty(client) == true))
	{
		this->_ev.events = EPOLLOUT;
		this->_ev.data.fd = fd;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd,
			&this->_ev) == -1)
			throw std::runtime_error("[Error] epoll_ctl_mod() failed");
	}
}

template <typename T>
void printMap(T & map, std::string const & name)
{
	typename	T::iterator	it;
	typename	T::iterator	end;

	std::cout << "----------------" << std::endl;
	std::cout << name << " contains:" << std::endl;

	end = map.end();
	for (it = map.begin() ; it != end ; it++)
		std::cout << it->first << " => " << it->second << std::endl;
	std::cout << "size = " << map.size() << std::endl;
	std::cout << "----------------\n" << std::endl;
}

void	Engine::loop_server(const std::vector<Server> & src)
{
	int	nbr_connexions = 0, i;
	struct	epoll_event events_fd[MAX_EVENTS];

	printMap(this->_port_fd, "fd");

	for (size_t e = 0 ; e < MAX_EVENTS ; ++e)
	{
		events_fd[e].data.fd = -1;
		events_fd[e].events = EPOLLIN;
	}

	while (true)
	{
		if ((nbr_connexions = epoll_wait(this->_epfd, events_fd,
			MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		for (i = 0 ; i < nbr_connexions ; ++i)
		{
			if (events_fd[i].events & EPOLLERR || events_fd[i].events & EPOLLHUP)
				delete_client(events_fd[i].data.fd);
			else if (events_fd[i].events & EPOLLIN && is_listener(events_fd[i].data.fd))
				accept_client(events_fd[i].data.fd);
			else if (events_fd[i].events & EPOLLIN)
				read_client(this->_client[events_fd[i].data.fd], events_fd[i].data.fd);
			else if (events_fd[i].events & EPOLLOUT)
			{
				send_client(src, this->_client[events_fd[i].data.fd], events_fd[i].data.fd);
				delete_client(events_fd[i].data.fd);
			}
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int			const & Engine::GetAccessPort() const
{
	return (this->_port);
}

std::string	const & Engine::GetRemote_Port() const
{
	return (this->_remote_port);
}

std::string	const & Engine::GetRemote_Addr() const
{
	return (this->_remote_addr);
}


/* ************************************************************************** */
