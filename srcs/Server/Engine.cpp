/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/17 18:58:04dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine.hpp"
#include "../Cgi/Cgi.hpp"
#include "../Parse_request/parse_request.hpp"
#include "Connexion.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Engine::Engine()
{
}

/*
** --------------------------------- PRIVATE METHODS ----------------------------------
*/

// Creating socket file descriptor
int	Engine::create_socket()
{
	int _listen_fd = 0;
	_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (_listen_fd);
}

// Set socket file descriptor to be reusable
void	Engine::set_socket(int listen_fd)
{
	int opt = 1;
	fcntl(listen_fd, F_SETFL, O_NONBLOCK);
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Check if port config is binded
bool	Engine::is_binded(int port_config)
{
	for (int i = 0; i < this->_i_server_binded; i++)
	{
		if (port_config == this->_port_binded[i])
			return (TRUE);
	}
	return (FALSE);
}

// Put a name to a socket
void	Engine::bind_socket(int listen_fd, const std::vector<Server> & src)
{
	int port_config = 0;

	std::istringstream(src[this->_i_server].getListen()) >> port_config;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port_config);
	std::cout << GREEN << "Port: " << port_config << std::endl << END;
	if (is_binded(port_config) == FALSE)
	{
		if (bind(listen_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr)) < 0)
			throw std::runtime_error("[Error] Bind failed");
		this->_port_binded[this->_i_server_binded] = port_config;
		this->_i_server_binded++;
	}
}

// Make the socket passive, waiting to accept
void	Engine::listen_socket(int listen_fd)
{
	if (listen(listen_fd, MAX_EVENTS) < 0)
		throw std::runtime_error("[Error] listen_socket() failed");
}

// Set remote addr and remote port
void	Engine::set_remote_var(struct sockaddr_in & addr_client)
{
	int i_remote_port = ntohs(addr_client.sin_port);

	std::stringstream ss;
	ss << i_remote_port;
	this->_remote_port = ss.str();
	this->_remote_addr = inet_ntoa(addr_client.sin_addr);
}

// Accept connexion and return socket accepted
int	Engine::accept_connexions(int listen_fd)
{
	struct sockaddr_in addr_client;
	int new_socket = 0;
	int client_len = sizeof(addr_client);

	new_socket = accept(listen_fd, (struct sockaddr *)&addr_client, (socklen_t *)&client_len);
	set_remote_var(addr_client);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	return (new_socket);
}

// savoir si le fd dans le epoll est un listener (socket d'un port) ou non
bool	Engine::is_listener(int fd, int *tab_fd, int nbr_servers, const std::vector<Server> & src)
{
	for (int i = 0; i < nbr_servers; i++)
	{
		if (fd == tab_fd[i])
		{
			std::istringstream(src[i].getListen()) >> this->_port;
			return (TRUE);
		}
	}
	return (FALSE);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


Engine::Engine(const std::vector<Server> & src)
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server(src);
	loop_server(src);
}

Engine::Engine( Engine const & src )
{
	*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Engine::~Engine()
{
	//std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Engine&				Engine::operator=( Engine const & rhs )
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
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Engine::setup_socket_server(const std::vector<Server> & src)
{
	this->_port = 0, this->_i_server_binded = 0;
	this->_nbr_servers = src.size();
	this->_timeout = 3 * 60 * 1000; // 3 min de _timeout (= keepalive nginx ?)
	this->_epfd = epoll_create(MAX_EVENTS);
	if (this->_epfd < 0)
		throw std::runtime_error("[Error] epoll_create() failed");
	for (this->_i_server = 0; this->_i_server < this->_nbr_servers; this->_i_server++)
	{
		this->_listen_fd[this->_i_server] = create_socket();
		this->_fds_events[this->_i_server].data.fd = this->_listen_fd[this->_i_server];
		this->_fds_events[this->_i_server].events = EPOLLIN;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_listen_fd[this->_i_server], &this->_fds_events[this->_i_server]) == -1)
			throw std::runtime_error("[Error] epoll_ctl_add() failed");
		set_socket(this->_listen_fd[this->_i_server]);
		bind_socket(this->_listen_fd[this->_i_server], src);
		listen_socket(this->_listen_fd[this->_i_server]);
	}
}

void	Engine::read_send_data(int i, int new_socket, const std::vector<Server> & src, Parse_request parse_head[MAX_EVENTS], Connexion	connexion[MAX_EVENTS])
{
	int		f;
	int		valread = -1;
	//connexion[i].b = 0;
	//else if(connexion[i].fill_request.find("\r\n\r\n") != std::string::npos)
	//if (valread != 0 && connexion[i].fill_request.find("\r\n\r\n") == std::string::npos )//&& e <= 80)
	if (this->_fds_events[i].events == EPOLLIN && connexion[i].is_parsed == false)
	{
		valread = recv(this->_fds_events[i].data.fd, & connexion[i].b, 1, 0);
		if (valread == -1)
			throw std::runtime_error("[Error] recv() failed");		
		else if (!((connexion[i].b == 13 || connexion[i].b == 10) && connexion[i].fill_request.size() == 0))
		{
			connexion[i].recv_len += valread;
			connexion[i].fill_request +=  connexion[i].b;
		}
		std::cout << connexion[i].b;


		if (connexion[i].fill_request.find("\r\n\r\n") != std::string::npos) // header rempli
		{
			//std::cout << "{EPOLLOUT}" << std::endl;
			this->_fds_events[i].events = EPOLLOUT;
			if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, new_socket, &this->_fds_events[i]) == -1)
				throw std::runtime_error("[Error] epoll_ctl_mod() failed");
			//std::cout << RED << " this->_fds_events=[" << this->_fds_events[i].events << "]";
			connexion[i].request_header_size = connexion[i].fill_request.size();
		}
		else if (connexion[i].fill_request.find("\r\n") != std::string::npos && parse_head[i].first_line_is_parsed == false)
		{
			std::cout << "{parse first line}" << std::endl;
			parse_head[i].parse_first_line(connexion[i].fill_request);
			parse_head[i].first_line_is_parsed = true;
			if(parse_head[i].get_request("Status") != "200")
			{
				send_data(valread, this->_fds_events[i].data.fd, src, parse_head[i]);
				parse_head[i] = Parse_request();
				connexion[i] = Connexion();
			}
		} 
	}
	else if (connexion[i].fill_request.find("\r\n\r\n") != std::string::npos && connexion[i].is_parsed == false)
	{
		
		//std::cout << YELLOW << "element=[" << connexion[i].fill_request << "]" << END << std::endl;
		//std::cout << YELLOW << "client n*[" << i << "]" << END << std::endl;
		std::cout << "{parse first request}" << std::endl;
		f = parse_head[i].parse_request(connexion[i].fill_request);
		connexion[i].is_parsed = true;
	}
	else if (connexion[i].fill_request.find("\r\n\r\n") != std::string::npos && connexion[i].is_parsed == true)
		read_request_body(i, src, parse_head, connexion);
}

void	Engine::read_request_body(int i, const std::vector<Server> & src, Parse_request parse_head[MAX_EVENTS], Connexion connexion[MAX_EVENTS])
{
	int valread = -1;
	int f;

	if (parse_head[i].get_request("Content-Length:") != ""  && connexion[i].is_sendable == false)
	{
		if (parse_head[i].get_request("Expect:") == "100-continue"
		&& parse_head[i].get_request("Transfer-Encoding:") == "chunked")
		{
			std::cout << "{else if}" << std::endl;
			//send(this->_fds_events[i].data.fd, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
			if (valread != 0 && connexion[i].fill_request.find("0\r\n\r\n") == std::string::npos)
			{
				valread = recv(this->_fds_events[i].data.fd, & connexion[i].b, 1, 0);
				connexion[i].recv_len += valread;
				connexion[i].fill_request +=  connexion[i].b;
			}
			else
			{
				std::cout << "{parse first body 1}" << std::endl;
				f = parse_head[i].parse_body(connexion[i].fill_request);
				connexion[i].is_sendable = true;
			}
		}
		else
		{
			if (valread != 0
			&& connexion[i].fill_request.size() < connexion[i].request_header_size + std::stoi(parse_head[i].get_request("Content-Length:")))
			{
				valread = recv(this->_fds_events[i].data.fd, & connexion[i].b, 1, 0);
				connexion[i].recv_len += valread;
				connexion[i].fill_request +=  connexion[i].b;
			}
			else
			{
				std::cout << "{parse first body 2}" << std::endl;
				f = parse_head[i].parse_body(connexion[i].fill_request);
				connexion[i].is_sendable = true;
			}
		}
	}
	else
	{
		send_data(valread, this->_fds_events[i].data.fd, src, parse_head[i]);
		parse_head[i] = Parse_request();
		connexion[i] = Connexion();
		//close(this->_fds_events[i].data.fd);
	}
}

void	Engine::send_data(int valread, int fd,const std::vector<Server> & src, Parse_request & parse_head)
{
	int		nbr_bytes_send = 0;

	//std::cout << YELLOW << "body=[" << parse_head.get_request_body() << "]" << END << std::endl;

 	if (valread != 0)
	{
		TreatRequest	treatment(src, *this);
		this->_buff_send = treatment.treat(parse_head);

		//epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout);
		nbr_bytes_send = send(fd, this->_buff_send.c_str(), this->_buff_send.size(), 0);
		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	}
	if (parse_head.get_request("Connection:") != "keep-alive")
		close(fd);
}


void	Engine::loop_server(const std::vector<Server> & src)
{
	int nbr_connexions = 0, new_socket = 0, i = 0;
	Parse_request	parse_head[MAX_EVENTS];
	Connexion		connexion[MAX_EVENTS];



	while (TRUE)
	{
		//std::cout << "{wait here}" << std::endl;
		if ((nbr_connexions = epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		//std::cout << "{wait here here}" << std::endl;
		//std::cout << RED << "nbr_connexions=[" << nbr_connexions << "]" << END << std::endl;
		for (i = 0; i < nbr_connexions; i++)
		{
			//std::cout << BLUE << "element=[" << is_listener(this->_fds_events[i].data.fd, this->_listen_fd, this->_nbr_servers, src) << "]" << END << std::endl;
			if (is_listener(this->_fds_events[i].data.fd, this->_listen_fd, this->_nbr_servers, src))
			{
				new_socket = accept_connexions(this->_fds_events[i].data.fd);
				this->_fds_events[i].events = EPOLLIN;
				std::cout << "{epol accept}" << std::endl;
				this->_fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket, &this->_fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
			}
			else
			{
				//std::cout << "{read send data}" << std::endl;
				read_send_data(i, new_socket, src, parse_head, connexion);
			}			
		}
	}

}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

int			Engine::GetAccessPort() const
{
	return (this->_port);
}

std::string	Engine::GetRemote_Port() const
{
	return (this->_remote_port);
}

std::string	Engine::GetRemote_Addr() const
{
	return (this->_remote_addr);
}
/* ************************************************************************** */
