/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g4g.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:27:13 by dodjian           #+#    #+#             */
/*   Updated: 2021/12/09 18:41:48 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket programming
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

void	display_all_fd_events(struct pollfd fds[200])
{
	for (int i = 0; i < 200 && fds[i].fd > 0; i++)
	{
		std::cout << "fds[" << i << "] = " << fds[i].fd << std::endl;
	}
}

void	loop_server(int new_socket, struct pollfd fds[200])
{
	int valread, i_conn = 0;
	char buffer[10000];

	bzero(&buffer, sizeof(buffer));
	display_all_fd_events(fds);
	while (1)
	{
		//valread = read( new_socket , buffer, 1024);
		valread = recv(fds[i_conn].fd, buffer, sizeof(buffer), 0);
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
		int nbr_bytes_send = 0;
		nbr_bytes_send = send(fds[i_conn].fd, file.c_str(), file.size(), 0);
		if (nbr_bytes_send == -1)
		{
			std::cout << "Sent failed" << std::endl;
			break ;
		}
		std::cout << "Hello message sent" << std::endl;
		i_conn++;
	}
	//send(new_socket , hello , strlen(hello) , 0 );
}

int main(void)
{
	int server_fd, new_socket = 0;
	struct pollfd fds[200];
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	//char *hello = "Hello from server";

	// Creating socket file descriptor
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	//new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	for (int nbr_connexions = 0; nbr_connexions < 3; nbr_connexions++) // 3 est fixe
	{
		new_socket = accept(server_fd, NULL, NULL);
		std::cout << "new_socket = " << new_socket << std::endl;
		if (new_socket < 0)
		{
			perror("accept");
			break ;
		}
		fds[nbr_connexions].fd = new_socket;
	}
	//display_all_fd_events(fds);
	loop_server(new_socket, fds);
	return 0;
}
