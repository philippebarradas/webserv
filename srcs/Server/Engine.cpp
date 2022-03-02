/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2022/03/02 15:24:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Engine::Engine()
{
}

Engine::Engine(const std::vector<Server> & src)
{
	std::cout << BBLUE "-------- Starting webserv --------\n" END << std::endl;
	setup_socket_server(src);
	loop_server(src);
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
	std::cout << BBLUE "\n--------- End of webserv ---------" END << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Engine&				Engine::operator=(const Engine & rhs)
{
	if (this != &rhs)
	{
		this->_addr = rhs._addr;
		this->_buff_send = rhs._buff_send;
		this->_epfd = rhs._epfd;
		this->_i_server = rhs._i_server;
		this->_i_server_binded = rhs._i_server_binded;
		this->_nbr_servers = rhs._nbr_servers;
		this->_port = rhs._port;
		this->_remote_addr = rhs._remote_addr;
		this->_remote_port = rhs._remote_port;
		this->_timeout = rhs._timeout;
		this->_valread = rhs._valread;
		this->_v = rhs._v;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

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

	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

bool	Engine::is_binded(const int & port_config)
{
	for (size_t i = 0; i < this->_i_server_binded; i++)
	{
		if (port_config == this->_port_binded[i])
			return (true);
	}
	return (false);
}

void	Engine::bind_socket(const int & listen_fd,
	const std::vector<Server> & src)
{
	int	port_config = 0;
	std::cout << RED << "listen_fd = ["<< listen_fd << "]" << END << std::endl;
	std::istringstream(src[this->_i_server].getListen()) >> port_config;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port_config);
	std::cout << PURPLE2 << "[webserv] listen on: " << port_config
		<< std::endl << END;
	if (is_binded(port_config) == false)
	{
		if (bind(listen_fd, (struct sockaddr *)&this->_addr,
			sizeof(this->_addr)) < 0)
			throw std::runtime_error("[Error] Bind failed");
		this->_port_binded[this->_i_server_binded] = port_config;
		this->_i_server_binded++;
	}
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
	std::cout << PURPLE2 << "[webserv] new client accept on: " << this->_port
		<< std::endl << END;
	set_remote_var(addr_client);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	return (new_socket);
}

bool	Engine::is_listener(const int & fd, const int *tab_fd,
	const int & nbr_servers, const std::vector<Server> & src)
{
	for (int i = 0; i < nbr_servers; i++)
	{
		if (fd == tab_fd[i])
		{
			std::istringstream(src[i].getListen()) >> this->_port;
			return (true);
		}
	}
	return (false);
}

bool	Engine::is_body_empty(Client & client)
{
	if (client.getParse_head().get_request("Content-Length:") != "" ||
		client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
		return (false);
	return (true);
}

void	Engine::setup_socket_server(const std::vector<Server> & src)
{
	this->_v.reserve(MAX_EVENTS);
	this->_port = 0, this->_i_server_binded = 0, this->_valread = -1;
	this->_nbr_servers = src.size();
	this->_timeout = 3 * 60 * 1000;
	this->_epfd = epoll_create(MAX_EVENTS);
	if (this->_epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	for (this->_i_server = 0; this->_i_server < this->_nbr_servers;
		this->_i_server++)
	{
		this->_listen_fd[this->_i_server] = create_socket();
		this->_fds_events[this->_i_server].data.fd =
			this->_listen_fd[this->_i_server];
		this->_fds_events[this->_i_server].events = EPOLLIN;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD,
			this->_listen_fd[this->_i_server],
				&this->_fds_events[this->_i_server]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->_listen_fd[this->_i_server]);
		bind_socket(this->_listen_fd[this->_i_server], src);
		listen_socket(this->_listen_fd[this->_i_server]);
	}
	std::cout << std::endl;
}

void	Engine::read_header(Client & client)
{
	char	b;

	_valread = recv(client.getEvents().data.fd, &b, 1, 0);
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

void Engine::read_content_length(Client & client)
{
	size_t length_body =  stost_size(0, MAX_MAXBODY,
		client.getParse_head().get_request("Content-Length:"), "_request_body_size");
	size_t length_request = (client.getRequest_header_size() + length_body);
	if (client.getFill_request().size() < length_request)
	{
		if (client.getFill_request().size() + BUFFER_SIZE > length_request)
			_valread = recv(client.getEvents().data.fd, &_buff,
			length_request - (client.getFill_request().size()), 0);
		else
			_valread = recv(client.getEvents().data.fd, &_buff, BUFFER_SIZE, 0);
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

void Engine::read_chunked(Client & client)
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
			_valread = recv(client.getEvents().data.fd, &b, 1, 0);
			_length_chunk_string += b;
			client.setRecv_len(_valread);
			client.setFill_request(b);
		}
		else
		{
			_valread = recv(client.getEvents().data.fd, &_buff_chunked, _length_chunk, 0);
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

void	Engine::read_body(Client & client)
{
	bzero(_buff, BUFFER_SIZE);
	bzero(_buff_chunked, BUFFER_SIZE_CHUNKED);
	if (client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
		read_chunked(client);
	else
		read_content_length(client);
}

void	Engine::send_data(const std::vector<Server> & src, Client & client)
{
	int		nbr_bytes_send = 0;

	if (_valread != 0)
	{
		TreatRequest	treatment(src, *this);
		this->_buff_send = treatment.treat(client.getParse_head());
		nbr_bytes_send = send(client.getEvents().data.fd,
			this->_buff_send.c_str(), this->_buff_send.size(), 0);

		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
	}
}

void	Engine::loop_accept(const int & nbr_connexions,
	const std::vector<Server> & src)
{
	int	new_socket = 0, i = 0;
	for (i = 0; i < nbr_connexions; i++)
	{
		if (is_listener(this->_fds_events[i].data.fd, this->_listen_fd,
			this->_nbr_servers, src))
		{
			new_socket = accept_connexions(this->_fds_events[i].data.fd);
			this->_fds_events[i].events = EPOLLIN;
			this->_fds_events[i].data.fd = new_socket;
			if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket,
				&this->_fds_events[i]) == -1)
				throw std::runtime_error("[Error] epoll_ctl_add() failed");
			this->_v.push_back(Client(this->_fds_events[i]));
		}
	}
}

void	Engine::myRead(Client & client)
{
	if (client.getHeader_readed() == false)
		read_header(client);
	if (client.getHeader_parsed() == false && client.getHeader_readed() == true)
	{
		client.getParse_head().parse_request(client.getFill_request());
		client.setHeader_parsed(true);
	}
	else if (is_body_empty(client) == false)
		read_body(client);
	if (client.getIs_sendable() == true
		|| client.getParse_head().error_first_line == true
		|| (client.getHeader_parsed() == true && is_body_empty(client) == true))
	{
		client.getEvents().events = EPOLLOUT;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, client.getEvents().data.fd,
			&client.getEvents()) == -1)
			throw std::runtime_error("[Error] epoll_ctl_mod() failed");
	}
}

void	Engine::loop_input_output(const std::vector<Server> & src)
{
	std::vector<Client>::iterator	it;

	for (it = _v.begin(); it != _v.end(); ++it)
	{
		if (it->getEvents().events & EPOLLERR)
		{
			std::cout << "EPOLLERR" << std::endl;
			send_data(src, *it);
			close(it->getEvents().data.fd);
			it = _v.erase(it);
			if (it == _v.end())
				break ;
		}
		else if (it->getEvents().events & EPOLLIN)
			myRead(*it);
		else if (it->getEvents().events & EPOLLOUT)
		{
			send_data(src, *it);
			if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, it->getEvents().data.fd,
				&it->getEvents()) == -1)
				throw std::runtime_error("[Error] epoll_ctl_del() failed");	
			close(it->getEvents().data.fd);
			it = _v.erase(it);
			if (it == _v.end())
				break ;
		}
	}
}

void	Engine::loop_server(const std::vector<Server> & src)
{
	int	nbr_connexions = 0;
	while (true)
	{
		if ((nbr_connexions = epoll_wait(this->_epfd, this->_fds_events,
			MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		loop_accept(nbr_connexions, src);
		loop_input_output(src);
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
