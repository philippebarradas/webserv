/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsannie <tsannie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 10:09:47 by tsannie           #+#    #+#             */
/*   Updated: 2021/11/30 1:03:52 by tsannie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define SERVER_PORT  12345

#define TRUE             1
#define FALSE            0

int	main (void)
{
	int					len, sockfd, sockfd2, on2 = 1, on = 1;
	int					listen_sd, listen_sd2 = -1, new_sd = -1;
	int					end_server = FALSE, compress_array = FALSE;
	int					close_conn;
	char				buffer[80];
	struct sockaddr_in	addr;
	int					timeout;
	struct epoll_event	events[200];
	int					nfds = 1, current_size = 0, i, j;

	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming      */
	/* connections on                                            */
	/*************************************************************/
	listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	listen_sd2 = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		perror("socket() failed");
		exit(-1);
	}
	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	sockfd = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
		(char *)&on, sizeof(on));
	sockfd2 = setsockopt(listen_sd2, SOL_SOCKET,  SO_REUSEADDR,
		(char *)&on2, sizeof(on2));
	if (sockfd < 0)
	{
		perror("setsockopt() failed");
		close(listen_sd);
		exit(-1);
	}
	sockfd = fcntl(listen_sd, F_SETFL, O_NONBLOCK);
	sockfd2 = fcntl(listen_sd2, F_SETFL, O_NONBLOCK);

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port        = htons(SERVER_PORT); // 12345
	sockfd = bind(listen_sd,
		(struct sockaddr *)&addr, sizeof(addr));
	addr.sin_port        = htons(1111);
	sockfd2 = bind(listen_sd2,
		(struct sockaddr *)&addr, sizeof(addr));
	if (sockfd < 0)
	{
		perror("bind() failed");
		close(listen_sd);
		exit(-1);
	}

	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/
	sockfd = listen(listen_sd, 32);
	sockfd2 = listen(listen_sd2, 32);
	if (sockfd < 0)
	{
		perror("listen() failed");
		close(listen_sd);
		exit(-1);
	}

	/*************************************************************/
	/* Initialize the pollfd structure                           */
	/*************************************************************/

	/*************************************************************/
	/* Set up the initial listening socket                        */
	/*************************************************************/

	events[0].data.fd = listen_sd;
	events[0].events = EPOLLIN;
	/*************************************************************/
	/* Initialize the timeout to 3 minutes. If no                */
	/* activity after 3 minutes this program will end.           */
	/* timeout value is based on milliseconds.                   */
	/*************************************************************/
	timeout = (3 * 60 * 1000);

	/*************************************************************/
	/* Loop waiting for incoming connects or for incoming data   */
	/* on any of the connected sockets.                          */
	/*************************************************************/
	do
	{
	/***********************************************************/
	/* One or more descriptors are readable.  Need to          */
	/* determine which ones they are.                          */
	/***********************************************************/
	current_size = nfds;
	for (i = 0; i < current_size; i++)
	{
		/*********************************************************/
		/* Loop through to find the descriptors that returned    */
		/* POLLIN and determine whether it's the listening       */
		/* or the active connection.                             */
		/*********************************************************/
		if(events[i].events == 0)
			continue;

		/*********************************************************/
		/* If revents is not POLLIN, it's an unexpected result,  */
		/* log and end the server.                               */
		/*********************************************************/
		if(events[i].events != EPOLLIN)
		{
			printf("  Error! revents = %d\n", events[i].events);
			end_server = TRUE;
			break;
		}
		if (events[i].data.fd == listen_sd)
		{
			do
			{
				new_sd = accept(listen_sd, NULL, NULL);

				if (new_sd < 0)
				{
					if (errno != EWOULDBLOCK)
					{
						perror("  accept() failed");
						end_server = TRUE;
					}
					break;
				}
				sockfd = epoll_create(1);
				printf("Waiting on poll()...\n");
				if (sockfd < 0)
				{
					perror("  poll() failed");
					break;
				}
				if (sockfd == 0)
				{
					printf("  poll() timed out.  End program.\n");
					break;
				}
				printf("  Listening socket is readable\n");
				epoll_ctl(new_sd, EPOLL_CTL_ADD, 0, events);
				/*****************************************************/
				/* Add the new incoming connection to the            */
				/* pollfd structure                                  */
				/*****************************************************/
				printf("  New incoming connection - %d\n", new_sd);
				events[nfds].data.fd = new_sd;
				events[nfds].events = EPOLLIN;
				nfds++;

				/*****************************************************/
				/* Loop back up and accept another incoming          */
				/* connection                                        */
				/*****************************************************/
			} while (new_sd != -1);
		}

	/*********************************************************/
	/* This is not the listening socket, therefore an        */
	/* existing connection must be readable                  */
	/*********************************************************/

	else
	{
		epoll_wait(events[i].data.fd, events, 200, timeout);
		printf("  Descriptor %d is readable\n", events[i].data.fd);
		close_conn = FALSE;
		do
		{
		sockfd = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
		printf(" nbr bytes read: %d\n", sockfd);
		if (sockfd < 0) // nbr of bytes read
		{
			if (errno != EWOULDBLOCK)
			{
				perror("  recv() failed");
				close_conn = TRUE;
			}
			break;
		}

		/*****************************************************/
		/* Check to see if the connection has been           */
		/* closed by the client                              */
		/*****************************************************/
		if (sockfd == 0)
		{
			printf("  Connection closed\n");
			close_conn = TRUE;
			break;
		}

		/*****************************************************/
		/* Data was received                                 */
		/*****************************************************/
		len = sockfd;
		printf("  %d bytes received\n", len);

		/*****************************************************/
		/* Echo the data back to the client                  */
		/*****************************************************/
		sockfd = send(events[i].data.fd, buffer, len, 0);
		if (sockfd < 0)
		{
			perror("  send() failed");
			close_conn = TRUE;
			break;
		}

		} while(TRUE);

		/*******************************************************/
		/* If the close_conn flag was turned on, we need       */
		/* to clean up this active connection. This            */
		/* clean up process includes removing the              */
		/* descriptor.                                         */
		/*******************************************************/
		if (close_conn)
		{
			close(events[i].data.fd);
			events[i].data.fd = -1;
			compress_array = TRUE;
		}


	}  /* End of existing connection is readable             */
	} /* End of loop through pollable descriptors              */

	/***********************************************************/
	/* If the compress_array flag was turned on, we need       */
	/* to squeeze together the array and decrement the number  */
	/* of file descriptors. We do not need to move back the    */
	/* events and revents fields because the events will always*/
	/* be POLLIN in this case, and revents is output.          */
	/***********************************************************/
	if (compress_array)
	{
		compress_array = FALSE;
		for (i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == -1)
			{
				for(j = i; j < nfds; j++)
				{
					events[j].data.fd = events[j+1].data.fd;
				}
				i--;
				nfds--;
			}
		}
	}

	} while (end_server == FALSE); /* End of serving running.    */

	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++)
	{
		if(events[i].data.fd >= 0)
			close(events[i].data.fd);
	}
}
