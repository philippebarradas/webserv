/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 10:09:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/11/30 12:18:56 by tsannie          ###   ########.fr       */
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
	int sockfd;
	struct sockaddr_in sin;

	//sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(2000);

    inet_aton("63.161.169.137", &sin.sin_addr.s_addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(sockfd, (struct sockaddr *)sin, sizeof(sin));

	/* ... */

	return 0;
}
