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
	std::cout << GREEN << "Port: " << port_config << std::endl << END;
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

	std::cout << "listen fd = " << listen_fd << std::endl;
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

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Engine::Engine()
{
}

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
	std::cout << GREEN << "----------------- End of server -----------------" << END << std::endl << std::endl;
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

void	Engine::read_header(int new_socket, const std::vector<Server> & src, Client & client)
{
	char	b;

	bzero(_buff, BUFFER_SIZE);
	_valread = recv(client.getEvents().data.fd, &b, 1, 0);
	//std::cout << "------------- READ ------------------" << std::endl;
	if (_valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	else
		client.recv_len += _valread;
	client.fill_request += b;
	if (client.fill_request.find("\r\n\r\n") != std::string::npos) // header rempli
	{
		//std::cout << "it->fill_request\t=\t" << client.fill_request << std::endl;
		//std::cout << "{EPOLLOUT}" << std::endl;
		if (client.getParse_head().get_request("Content-Length:") == "") // pas de body
		{
			client.getEvents().events = EPOLLOUT;
			if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, new_socket, &client.getEvents()) == -1)
				throw std::runtime_error("[Error] epoll_ctl_mod() failed");
		}
		//std::cout << RED << " this->_fds_events=[" << this->_fds_events[i].events << "]";
		client.request_header_size = client.fill_request.size();
	}
}

void	Engine::read_body(const std::vector<Server> & src, Client & client)
{
	char b;
	int f;

	//std::cout << "je suis dans read request body" << std::endl;
	//std::cout << "_buff\t=\t" << _buff << std::endl;
	if (client.getParse_head().get_request("Expect:") == "100-continue"
		&& client.getParse_head().get_request("Transfer-Encoding:") == "chunked")
	{
		std::cout << "{else if}" << std::endl;
		//send(this->_fds_events[i].data.fd, "HTTP/1.1 100 Continue\r\n\r\n", 25, 0);
		if (_valread != 0 && client.fill_request.find("0\r\n\r\n") == std::string::npos)
		{
			_valread = recv(client.getEvents().data.fd, &b, 1, 0);
			client.recv_len += _valread;
			client.fill_request += b;
		}
		else
		{
			f = client.getParse_head().parse_request_buffer(client.fill_request);
			client.is_sendable = true;
		}
	}
	else
	{
		//std::cout << YELLOW << "client.fill_request.size()\t=\t" << client.fill_request << END << std::endl;
		//std::cout << "taille header + c length = " << client.request_header_size +
			//std::stoi(client.getParse_head().get_request("Content-Length:")) << std::endl;
		if (_valread != 0
		&& client.fill_request.size() < client.request_header_size +
			std::stoi(client.getParse_head().get_request("Content-Length:")))
			//&& client.fill_request.find("\r\n", client.request_header_size) == std::string::npos)
		{
			_valread = recv(client.getEvents().data.fd, &b, 1, 0);
			//std::cout << "_valread\t=\t" << _valread << std::endl;
			client.recv_len += _valread;
			client.fill_request += b;
			//printf("%d\n", b);
		}
		else
		{
			std::cout << "j'ai read le body" << std::endl;
			f = client.getParse_head().parse_request_buffer(client.fill_request);
			client.is_sendable = true;
		}
	}
	//std::cout << "------------- READ ------------------" << std::endl;
}

void	Engine::send_data(int valread, const std::vector<Server> & src, Client & client)
{
	int		nbr_bytes_send = 0;
	/* if (_buff_send != "")
		_buff_send = "";
 */
	//std::cout << YELLOW << "body=[" << client.getParse_head().get_request_body() << "]" << END << std::endl;
	if (_valread != 0)
	{
		TreatRequest	treatment(src, *this);
		this->_buff_send = treatment.treat(client.getParse_head());
		//std::cout << "this->_buff_send\t=\t" << this->_buff_send << std::endl;
		std::cout << "AVANT LE SEND" << std::endl;
		//epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout);
		std::cout << "client fd\t=\t" << client.getEvents().data.fd << std::endl;
		nbr_bytes_send = send(client.getEvents().data.fd, this->_buff_send.c_str(), this->_buff_send.size(), 0);

		//if (nbr_bytes_send == -1)
			//throw std::runtime_error("[Error] sent() failed");
		std::cout << RED << "End of connexion" << END << std::endl << std::endl;
	}
	//close(fd);
}

template <typename T>
void printMap(T & map, std::string const & name)
{
	typename	T::iterator	it;
	typename	T::iterator	end;

	//std::cout << "----------------" << std::endl;
	//std::cout << name << " contains:" << std::endl;

	end = map.end();
	for (it = map.begin() ; it != end ; it++)
		std::cout << it->first << " => " << it->second << std::endl;
	std::cout << "size = " << map.size() << std::endl;
	std::cout << "----------------\n" << std::endl;
}

void	Engine::loop_server(const std::vector<Server> & src)
{
	this->_valread = -1;
	int nbr_connexions = 0, new_socket = 0, i = 0;

	//Parse_request	parse_head;
	//Client		client;
	//Client		*client = NULL;

	//Client		client[MAX_EVENTS];
	std::vector<Client> v;
	std::vector<Client>::iterator it, end;
	v.reserve(MAX_EVENTS);
	while (true) // serveur se lance
	{
		if ((nbr_connexions = epoll_wait(this->_epfd, this->_fds_events, MAX_EVENTS, this->_timeout)) < 0)
			throw std::runtime_error("[Error] epoll_wait() failed"); // return nbr connexions
		/* for (int i = 0; i < 300 && _fds_events[i].data.fd > 0; i++)
		{
			std::cout << PURPLE2 << "_fds_events[i].data.fd\t=\t" << _fds_events[i].data.fd << std::endl << END;
		} */
		//std::cout << "------------- WAIT ------------------" << std::endl;
		for (i = 0; i < nbr_connexions; i++)
		{
			if (is_listener(this->_fds_events[i].data.fd, this->_listen_fd, this->_nbr_servers, src))
			{
				new_socket = accept_connexions(this->_fds_events[i].data.fd);
				this->_fds_events[i].events = EPOLLIN;
				std::cout << "{epol accept}" << std::endl;
				this->_fds_events[i].data.fd = new_socket;
				if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, new_socket, &this->_fds_events[i]) == -1)
					throw std::runtime_error("[Error] epoll_ctl_add() failed");
				std::cout << "bfr push" << std::endl;
				v.push_back(Client(this->_fds_events[i]));
				//std::cout << "v.size()\t=\t" << v.size() << std::endl;
				//std::cout << "after push" << std::endl;
				//std::map<std::string, std::string> pol = v.begin()->getParse_head().getBigMegaSuperTab();
				//printMap(pol, "Tableau de merde");
			}
		}
		/*for (it = v.begin(); it != v.end(); ++it)
			std::cout << "client selected = " << *it << std::endl;*/

		for (it = v.begin(); it != v.end(); ++it)
		{
			//std::cout << "client selected = " << *it << std::endl;
			//std::cout << "v.size()\t=\t" << v.size();
			//std::cout << " de " << it->getEvents().data.fd << std::endl;
			//std::cout << RED << "fill_request=[" << it->fill_request << "]" << END << std::endl;
			if (it->getEvents().events == EPOLLIN && it->is_parsed == false)
				read_header(new_socket, src, *it);
			else if (it->fill_request.find("\r\n\r\n") != std::string::npos && it->is_parsed == false)
			// on a tout read le header
			{
				//std::cout << "read fini" << std::endl;
				//std::cout << RED << "fill_request=[" << it->fill_request << "]" << END << std::endl;
				it->getParse_head().parse_request_buffer(it->fill_request);
				it->is_parsed = true;
				//std::cout << "it->is_parsed in read header\t=\t" << it->is_parsed << std::endl;
			}
			if (it->getParse_head().get_request("Content-Length:") != ""  && it->is_sendable == false)
				read_body(src, *it);
			else
			{
				if (it->getParse_head().get_request("Content-Length:") == "" && it->is_parsed == true)
					it->is_sendable = true;
				//if (it->getEvents().events == EPOLLOUT && it->is_sendable == true)
				if (it->is_sendable == true)
				{
					std::cout << "{ap send ELSE}" << std::endl;
					std::cout << "valread\t=\t" << _valread << std::endl;
					send_data(_valread, src, *it);
					std::cout << "Avant le erase" << std::endl;
					//std::cout << YELLOW << "xx[" << it->fill_request << "]" << END << std::endl;
					//if (it->getParse_head().get_request("Connection:") == "close")
					//{
					//if (this->_fds_events[i].data.fd > 0)
					//{
					std::cout << CYAN "Je suis avant le close" END << std::endl;
					close(it->getEvents().data.fd);
					std::cout << CYAN "Je suis closed" END << std::endl;
					it = v.erase(it);
					std::cout << CYAN "Je suis erased" END << std::endl;
					//it = v.begin();
					if (it == v.end())
						break ;
				}
			}
		}
		//std::cout << "lol" << std::endl;
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
