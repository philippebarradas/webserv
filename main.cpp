/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dodjian <dovdjianpro@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 10:09:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/11/30 12:58:15 by dodjian          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	main( void )
{
    int fd, sockfd;
    socklen_t addrlen;
	struct sockaddr_in sin;
	struct sockaddr_in client;

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(2000);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // crée un point de communication, et renvoie un fd
	bind(sockfd, reinterpret_cast<struct sockaddr *>(&sin), sizeof(sin)); // bind() affecte l'adresse spécifiée dans addr à la socket référencée par le descripteur de fichier sockfd
    listen(sockfd, 1); // marque la socket comme passive, socket qui sera utilisé pour accepter les connexions
    memset(&client, 0, sizeof(client));
    addrlen = sizeof(client);
    std::cout << "fd: " << fd << std::endl;
    fd = accept(sockfd,(struct sockaddr*)&client, &addrlen); // crée une nouvelle socket connecté grâce au sockfd au premier dans la file d'attente de sockfd à l'écoute 
    std::cout << "fd: " << fd << std::endl;
	return 0;
}
