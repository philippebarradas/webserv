/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moteur.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/12 17:35:24 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "moteur.hpp"

// Creating socket file descriptor
int	create_socket()
{
	int listen_fd = 0;
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == 0)
		throw std::runtime_error("[Error] create_socket() failed");
	return (listen_fd);
}

// Set socket file descriptor to be reusable
void	set_socket(int listen_fd)
{
	int opt = 1;
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[Error] set_socket() failed");
}

// Put a name to a socket
void	bind_socket(int listen_fd)
{
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0 )
		throw std::runtime_error("[Error] bind_socket() failed");
}

// Make the socket passive, waiting to accept
void	listen_socket(int listen_fd)
{
	if (listen(listen_fd, 3) < 0)
		throw std::runtime_error("[Error] bind_socket() failed");
}

// Set first pollfd with passive socket and first event to waiting read data
void	set_first_poll_events(int listen_fd, struct pollfd fds_events[MAX_EVENTS])
{
	for (int i = 0; i < MAX_EVENTS; i++) // init all fd to 0
		fds_events[i].fd = 0;
	fds_events[0].fd = listen_fd;
	fds_events[0].events = POLLIN;
}

// Accept connexions, create new socket connected and put it at the end of events pollfd
int	accept_connexions(int listen_fd, int nbr_connexions, struct pollfd fds_events[MAX_EVENTS])
{
	int new_socket = 0;

	new_socket = accept(listen_fd, NULL, NULL);
	if (new_socket < 0)
		throw std::runtime_error("[Error] accept_connexions() failed");
	fds_events[nbr_connexions + 1].fd = new_socket;
	fds_events[nbr_connexions + 1].events = POLLIN;
	return (new_socket);
}

// Read data from buffer for now (after it will be the request send by client)
void	read_data(int i_conn, struct pollfd fds_events[MAX_EVENTS])
{
	int valread = 0;
	char buffer[10000];

	bzero(&buffer, sizeof(buffer));
	valread = recv(fds_events[i_conn].fd, buffer, sizeof(buffer), 0);
	if (valread == -1)
		throw std::runtime_error("[Error] recv() failed");
	if (valread == 0)
		throw std::runtime_error("[Error] recv() finished");
}

// Send data to the client (telnet or browser)
void	send_data(int i_conn, struct pollfd fds_events[MAX_EVENTS])
{
	std::ifstream ifs;
	std::string	line, file;

	ifs.open("to_delete.html", std::ifstream::in);
	while (std::getline(ifs, line))
	{
		file += line;
		file += '\n';
	}
	ifs.close();
	fds_events[i_conn].events = POLLOUT;
	int nbr_bytes_send = 0;
	nbr_bytes_send = send(fds_events[i_conn].fd, file.c_str(), file.size(), 0);
	if (nbr_bytes_send == -1)
		throw std::runtime_error("[Error] sent() failed");
	std::cout << RED << "End of connexion" << std::endl << std::endl;
}

// Close connexions after sent and contract array
void	close_and_contract_array(int i_conn, int nbr_connexions, struct pollfd fds_events[MAX_EVENTS])
{
	close(fds_events[i_conn].fd);
	int j = 0;
	for (j = 0; j < i_conn; j++){}
	for (j = i_conn; fds_events[j + 1].fd > 0 && j < nbr_connexions; j++){}
		fds_events[j].fd = fds_events[j + 1].fd;
}

// I/O processing (read, send and close and contract array after closing connexions)
void	io_processing(int i_conn, int nbr_connexions, struct pollfd fds_events[MAX_EVENTS])
{
	read_data(i_conn, fds_events);
	send_data(i_conn, fds_events);
	close_and_contract_array(i_conn, nbr_connexions, fds_events);
}

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Webserv::Webserv()
{
	std::cout << BLUE << "----------------- Starting server -----------------" << std::endl << std::endl;
	setup_socket_server();
	loop_server(this->listen_fd);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Webserv::~Webserv()
{
	std::cout << GREEN << "----------------- End of server -----------------" << std::endl << std::endl;
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Webserv &				Webserv::operator=( Webserv const & rhs )
{
	(void)rhs;
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/* cree la socket -> set la socket -> donne un nom a la socket ->
	mets la socket comme passive -> set le premier events fd avec la socket passive */
void	Webserv::setup_socket_server()
{
	this->listen_fd = create_socket();
	set_socket(this->listen_fd);
	bind_socket(this->listen_fd);
	listen_socket(this->listen_fd);
	set_first_poll_events(this->listen_fd, this->fds_events);
}

// loop server with polling events
void	Webserv::loop_server(int listen_fd)
{
	//this->timeout = 3 * 60 * 1000; // 3 min de timeout (= keepalive nginx ?)
	this->nbr_connexions = 0;
	int i_conn = 0;
	int new_socket = 0;
	while (TRUE)
	{
		for (int i = 0; this->fds_events[i].fd > 0; i++)
			std::cout << PURPLE2 << "fds_events[" << i << "] = " << this->fds_events[i].fd << std::endl;
		poll(fds_events, MAX_EVENTS, -1); // -1 = delai en ms infini
		if (this->fds_events[i_conn].fd == listen_fd)
		{
			new_socket = accept_connexions(listen_fd, this->nbr_connexions, this->fds_events);
			this->nbr_connexions++;
		}
		else
		{
			io_processing(i_conn, this->nbr_connexions, this->fds_events);
			this->nbr_connexions--;
			i_conn = -1;
		}
		std::cout << YELLOW << "nbr_connexions = " << nbr_connexions << std::endl << std::endl;
		i_conn++;
	}
}

int	main( void )
{
	try
	{
		Webserv	serv;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */
