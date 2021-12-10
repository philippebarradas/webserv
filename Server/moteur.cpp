/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g4g.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/10 14:02:45 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <fstream>
#define PORT 8080

void	loop_accept(int listen_fd, int new_socket, int i_conn, struct pollfd fds[200])
{
	int nbr_connexions = 1;
	for (int nbr_connexions = 1; nbr_connexions < 4; nbr_connexions++) // 3 est fixe
	{
		new_socket = accept(listen_fd, NULL, NULL);
		std::cout << "new_socket = " << new_socket << std::endl;
		if (new_socket < 0)
		{
			perror("accept");
			break ;
		}
		fds[nbr_connexions].fd = new_socket;
		fds[nbr_connexions].events = POLLIN;
	}
}

void	loop_server(int listen_fd, int new_socket, struct pollfd fds[200])
{
	int valread = 0;
	int i_conn;
	int nbr_connexions = 0;
	char buffer[10000];

	bzero(&buffer, sizeof(buffer));
	while (1)
	{
		for (int i = 0; fds[i].fd > 0; i++)
			std::cout << "fds[" << i << "] = " << fds[i].fd << std::endl;
		poll(fds, 200, -1); // 100 = delai en ms tant qu'il se passe rien
		//std::cout << "i_conn = " << i_conn << std::endl;
		if (fds[i_conn].fd == listen_fd)
		//if (fds[i_conn].events == POLLIN)
		{
			//loop_accept(listen_fd, new_socket, i_conn, fds);
			//for (nbr_connexions = i_conn + 1; nbr_connexions < i_conn + 3; nbr_connexions++) // 3 est fixe
			//{
			new_socket = accept(listen_fd, NULL, NULL);
			std::cout << "new_socket = " << new_socket << std::endl;
			if (new_socket < 0)
			{
				perror("accept");
				std::cout << "new_socket = fefe" << new_socket << std::endl;
				break ;
			}
			fds[nbr_connexions + 1].fd = new_socket;
			fds[nbr_connexions + 1].events = POLLIN;
			nbr_connexions++;
			//}
		}
		else
		{
			std::cout << "else i_conn = " << i_conn << std::endl;
			//valread = read( new_socket , buffer, 1024);
			valread = recv(fds[i_conn].fd, buffer, sizeof(buffer), 0);
			if (valread == -1)
			{
				std::cout << "recv failed" << std::endl;
				break ;
			}
			if (valread == 0)
			{
				std::cout << "Connection closed" << std::endl;
				break ;
			}
			std::ifstream ifs;

			ifs.open("to_delete.html", std::ifstream::in);
			std::string	line;
			std::string	file;
			while (std::getline(ifs, line))
			{
				file += line;
				file += '\n';
			}
			ifs.close();
			//std::cout << file << std::endl;
			fds[i_conn].events = POLLOUT;
			int nbr_bytes_send = 0;
			nbr_bytes_send = send(fds[i_conn].fd, file.c_str(), file.size(), 0);
			if (nbr_bytes_send == -1)
			{
				std::cout << "Sent failed" << std::endl;
				break ;
			}
			std::cout << "Hello message sent" << std::endl;

			close(fds[i_conn].fd);
			int j = 0;
			for (j = 0; j < i_conn; j++){}
			for (j = i_conn; fds[j + 1].fd > 0 && j < nbr_connexions; j++){}
				fds[j].fd = fds[j + 1].fd;
			nbr_connexions--;
			i_conn = -1;
		}
		std::cout << "nbr_connexions = " << nbr_connexions << std::endl;
		i_conn++;
	}
	//send(new_socket , hello , strlen(hello) , 0 );
}

int main(void)
{
	int listen_fd, new_socket = 0;
	struct pollfd fds[200];
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	//char buffer[1024] = {0};
	//char *hello = "Hello from server";

	// Creating socket file descriptor
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "listen_fd = " << listen_fd << std::endl;
	if (listen_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(listen_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	fds[0].fd = listen_fd;
	fds[0].events = POLLIN;
	// Set listen_fd in events_fds tab
	//new_socket = accept(listen_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	//display_all_fd_events(fds);
	loop_server(listen_fd, new_socket, fds);
	return 0;
}
