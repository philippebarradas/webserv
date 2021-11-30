/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 16:11:52 by tsannie           #+#    #+#             */
/*   Updated: 2021/11/30 18:20:08 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#define	SERVER_PORT		12345

int	main( void )
{
	int		socketClient = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in	addr;
	char				buffer[80];

	addr.sin_addr.s_addr	= inet_addr("127.0.0.1");
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(SERVER_PORT);
	connect(socketClient, (const struct sockaddr *)&addr, sizeof(addr));
	std::cout << "Log in !" << std::endl;


	while (1)
	{
		bzero(&buffer, sizeof(buffer));
		read(0, &buffer, 80);
		send(socketClient, &buffer, sizeof(buffer), 0);
		recv(socketClient, &buffer, sizeof(buffer), 0);
		std::cout << buffer << std::endl;

	}

	/*recv(socketClient, &buffer, sizeof(buffer), 0);
	std::cout << buffer << std::endl;*/

	close(socketClient);
}
