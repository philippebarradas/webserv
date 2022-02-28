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
#include "Client.hpp"

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
			return (true);
	}
	return (false);
}

// Put a name to a socket
void	Engine::bind_socket(int listen_fd, const std::vector<Server> & src)
{
	int port_config = 0;

	std::istringstream(src[this->_i_server].getListen()) >> port_config;
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(port_config);
	//std::cout << GREEN << "Port: " << port_config << std::endl << END;
	if (is_binded(port_config) == false)
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

	//std::cout << "listen fd = " << listen_fd << std::endl;
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
			return (true);
		}
	}
	return (false);
}

// savoir si le body est vide
bool	Engine::is_body_empty(Client & client)
{
	if (client.getParse_head().get_request("Content-Length:") != "" ||
		client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
		return (false);
	return (true);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Engine::Engine()
{
}

Engine::Engine(const std::vector<Server> & src)
{
	std::cout << BPURPLE "-------- Starting webserv --------\n" END << std::endl;
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
	std::cout << BBLUE "\n--------- End of webserv ---------" END << std::endl;
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
		this->_v = rhs._v;
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
	this->_v.reserve(MAX_EVENTS);
	this->_port = 0, this->_i_server_binded = 0, this->_valread = -1;
	this->_nbr_servers = src.size();
	this->_timeout = 3 * 60 * 1000; // 3 min de _timeout
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

// read le header de la requete
void	Engine::read_header(const std::vector<Server> & src, Client & client)
{
	char	b;

	bzero(_buff, BUFFER_SIZE);
	_valread = recv(client.getEvents().data.fd, &b, 1, 0);
	if (_valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	else if (!((b == 13 || b == 10)
	&& client.getFill_request().size() == 0))
	{
		client.setRecv_len(_valread); // +=
		client.setFill_request(b); // +=
	}
	if (client.getFill_request().find("\r\n\r\n") != std::string::npos) // header rempli
	{
		client.setHeader_readed(true);
		client.setRequest_header_size(client.getFill_request().size());
	}
	else if (client.getFill_request().find("\r\n") != std::string::npos &&
	client.getParse_head().first_line_is_parsed == false)
	{
		//std::cout << "{parse first line}" << std::endl;
		client.getParse_head().parse_first_line(client.getFill_request());
		client.getParse_head().first_line_is_parsed = true;
 		if(client.getParse_head().get_request("Status") != "200")
			client.getParse_head().error_first_line = true;
	}

}

// read le body de la requete
void	Engine::read_body(const std::vector<Server> & src, Client & client)
{
	char b;

	if (client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
	{
		if (client.getFill_request().find("0\r\n\r\n") == std::string::npos)
		{
			_valread = recv(client.getEvents().data.fd, &b, 1, 0);
			client.setRecv_len(_valread); // +=
			client.setFill_request(b); // +=
		}
		if (client.getFill_request().find("0\r\n\r\n") != std::string::npos)
		{
			client.getParse_head().parse_body(client.getFill_request());
			client.setIs_sendable(true);
		}
	}
	else
	{
		if (client.getFill_request().size() < (client.getRequest_header_size() +
			stost_size(0, MAX_MAXBODY, client.getParse_head().get_request("Content-Length:"), "_request_body_size")))
		{
			_valread = recv(client.getEvents().data.fd, &b, 1, 0);
			// recv len += valread
			client.setRecv_len(_valread);
			client.setFill_request(b);
		}
		if (client.getFill_request().size() == (client.getRequest_header_size() +
			stost_size(0, MAX_MAXBODY, client.getParse_head().get_request("Content-Length:"), "_request_body_size")))
		{
			std::cout << "j'ai read le body" << std::endl;
			std::cout << RED << "[parse body content length]]" << END << std::endl;
			client.getParse_head().parse_body(client.getFill_request());
			client.setIs_sendable(true);
		}
	}
}

// traite et envoie la reponse au client
void	Engine::send_data(const std::vector<Server> & src, Client & client)
{
	int		nbr_bytes_send = 0;

	if (_valread != 0)
	{
		//std::cout << "------------- SEND ------------------" << std::endl;
		TreatRequest	treatment(src, *this);
		this->_buff_send = treatment.treat(client.getParse_head());
		nbr_bytes_send = send(client.getEvents().data.fd, this->_buff_send.c_str(), this->_buff_send.size(), 0);

		if (nbr_bytes_send == -1)
			throw std::runtime_error("[Error] sent() failed");
		//std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	}
}

// boucle pour accepter les connexions provenant des clients
void	Engine::loop_accept(int nbr_connexions, const std::vector<Server> & src)
{
	int	new_socket = 0;
	this->_fd_i = 0;
	for (_fd_i = 0; _fd_i < nbr_connexions; _fd_i++)
	{
		if (is_listener(this->_fds_events[_fd_i].data.fd, this->_listen_fd, this->_nbr_servers, src))
		{
			new_socket = accept_connexions(this->_fds_events[_fd_i].data.fd);
			this->_fds_events[_fd_i].events = EPOLLIN;
			this->_fds_events[_fd_i].data.fd = new_socket;
			if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket, &this->_fds_events[_fd_i]) == -1)
				throw std::runtime_error("[Error] epoll_ctl_add() failed");
			this->_v.push_back(Client(this->_fds_events[_fd_i]));
		}
	}
}

// read les datas de la requete
void	Engine::myRead(const std::vector<Server> & src, Client & client)
{
	if (client.getHeader_readed() == false)
		read_header(src, client);
	if (client.getHeader_parsed() == false && client.getHeader_readed() == true)
	// on a tout read le header
	{
		client.getParse_head().parse_request(client.getFill_request());
		client.setHeader_parsed(true);
	}
	else if (is_body_empty(client) == false)
		read_body(src, client);
	if (client.getIs_sendable() == true || client.getParse_head().error_first_line == true ||
		(client.getHeader_parsed() == true && is_body_empty(client) == true))
	{
		client.getEvents().events = EPOLLOUT;
		if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, client.getEvents().data.fd, &client.getEvents()) == -1)
			throw std::runtime_error("[Error] epoll_ctl_mod() failed");
	}
}

// boucle d'input output
void	Engine::loop_input_output(const std::vector<Server> & src)
{
	std::vector<Client>::iterator it;

	for (it = _v.begin(); it != _v.end(); ++it)
	{
		if (it->getEvents().events & EPOLLIN)
			myRead(src, *it);
		else if (it->getEvents().events & EPOLLOUT)
		{
			send_data(src, *it);
			if (it->getParse_head().get_request("Connection:") == "close")
			{
				close(it->getEvents().data.fd);
				it = _v.erase(it);
				if (it == _v.end())
					break ;
			}
			else
				it->reinit();
		}
	}
}

// boucle infini du serveur
void	Engine::loop_server(const std::vector<Server> & src)
{
	int nbr_connexions = 0;
	while (true) // serveur se lance
	{
		if ((nbr_connexions = epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed");
		loop_accept(nbr_connexions, src);
		loop_input_output(src);
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
