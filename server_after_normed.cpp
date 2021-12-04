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
#include <iostream>
#include <string.h>
//#include "option.c"

#define SERVER_PORT 12345
#define TRUE 1
#define FALSE 0

int	create_socket()
{
	int	listen_sd = -1;

	listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		perror("socket() failed");
		return (-1);
	}
	return (listen_sd);
}

int	set_socket(int listen_sd)
{
	int	rc, on = 1;

	rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(listen_sd);
		return (-1);
	}
	return (rc);
}

int	bind_socket(int rc, int listen_sd)
{
	struct	sockaddr_in   addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//memcpy(&addr.sin_addr, INADDR_ANY, sizeof(INADDR_ANY));
	addr.sin_port        = htons(SERVER_PORT);
	rc = bind(listen_sd,
			  (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
	  perror("bind() failed");
	  close(listen_sd);
	  return (-1);
	}
	return (rc);
}

int	my_poll(int rc, int nfds)
{
	printf("Waiting on epoll()...\n");
	rc = epoll_create(nfds);
	/***********************************************************/
	/* Check to see if the poll call failed.                   */
	/***********************************************************/
	if (rc < 0)
	{
		perror("  poll() failed");
		return (-1);
		//break;
	}
	/***********************************************************/
	/* Check to see if the 3 minute time out expired.          */
	/***********************************************************/
	if (rc == 0)
	{
		std::cout << "poll() timed out.  End program." << std::endl;
		return (-1);
		//break;
	}
	return (rc);
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////



int	ft_iff(int new_sd, int listen_sd, int end_server, int nfds, struct epoll_event events[200])
{
	/*******************************************************/
	/* Listening descriptor is readable.                   */
	/*******************************************************/
		printf("  Listening socket is readable\n");

	/*******************************************************/
	/* Accept all incoming connections that are            */
	/* queued up on the listening socket before we         */
	/* loop back and call poll again.                      */
	/*******************************************************/
	do
	{
		/*****************************************************/
		/* Accept each incoming connection. If               */
		/* accept fails with EWOULDBLOCK, then we            */
		/* have accepted all of them. Any other              */
		/* failure on accept will cause us to end the        */
		/* server.                                           */
		/*****************************************************/
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

		/*****************************************************/
		/* Add the new incoming connection to the            */
		/* pollfd structure                                  */
		/*****************************************************/
		printf("  New incoming connection - %d\n", new_sd);
		events[nfds].data.fd = new_sd;
		events[nfds].events = EPOLLIN;
		epoll_ctl(listen_sd, EPOLL_CTL_ADD, events[nfds].data.fd, &events[nfds]);
		nfds++;

		/*****************************************************/
		/* Loop back up and accept another incoming          */
		/* connection                                        */
		/*****************************************************/
	} while (new_sd != -1);

	return nfds;

}
	/*********************************************************/
	/* This is not the listening socket, therefore an        */
	/* existing connection must be readable                  */
	/*********************************************************/

void	ft_ellse(int close_conn, int rc, int len, char *buffer, int i, struct epoll_event events[200], int compress_array)
	{
		printf("  Descriptor %d is readable\n", events[i].data.fd);
		close_conn = FALSE;
		/*******************************************************/
		/* Receive all incoming data on this socket            */
		/* before we loop back and call poll again.            */
		/*******************************************************/

		do
		{
		/*****************************************************/
		/* Receive data on this connection until the         */
		/* recv fails with EWOULDBLOCK. If any other         */
		/* failure occurs, we will close the                 */
		/* connection.                                       */
		/*****************************************************/
		epoll_wait(rc, events, 200, 10000);
		rc = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
		if (rc < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				perror("  recv() failed");
				close_conn = TRUE;
			}
			break;
		}

		if (rc == 0)
		{
			printf("  Connection closed\n");
			close_conn = TRUE;
			break;
		}
		len = rc;
		printf("  %d bytes received\n", len);
		std::cout << "len = " << len << " rc = " << rc << " buffer = " << buffer << std::endl;

		/*****************************************************/
		/* Echo the data back to the client                  */
		/*****************************************************/
		rc = send(events[i].data.fd, buffer, len, 0);
		if (rc < 0)
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


void	ft_compress_array(int compress_array, int nfds, struct epoll_event events[200], int i, int j)
{
	if (compress_array)
	  {
		compress_array = FALSE;
		for (i = 0; i < nfds; i++)
		{
		  if (events[i].data.fd == -1)
		  {
			for(j = i; j < nfds; j++)
			{
			  events[j].data.fd = events[j + 1].data.fd;
			}
			i--;
			nfds--;
		  }
		}
	  }
}


void ft_close(int i, int nfds, struct epoll_event events[200])
{
	/*************************************************************/
	/* Clean up all of the sockets that are open                 */
	/*************************************************************/
	for (i = 0; i < nfds; i++)
	{
	  if(events[i].data.fd >= 0)
		close(events[i].data.fd);
	}
}

//////////////////////////////////////////////////////////////////////


//int main (int argc, char *argv[])
int main(void)
{
	int	len = 0, rc = 1;
	int	listen_sd = -1, new_sd = -1;
	int	end_server = FALSE, compress_array = FALSE;
	int	close_conn = 0;
	char	buffer[80];
	//struct	sockaddr_in   addr;
	int	timeout;
	struct	epoll_event events_var[200];
	int	nfds = 1, current_size = 0, i = 0, j = 0;
	/*************************************************************/
	/* Create an AF_INET stream socket to receive incoming      */
	/* connections on                                            */
	/*************************************************************/
	listen_sd = create_socket();
	if (listen_sd < 0)
		return (-1);
	/*************************************************************/
	/* Allow socket descriptor to be reuseable                   */
	/*************************************************************/
	rc = set_socket(listen_sd);
	if (rc < 0)
		return (-1);

	/*************************************************************/
	/* Set socket to be nonblocking. All of the sockets for      */
	/* the incoming connections will also be nonblocking since   */
	/* they will inherit that state from the listening socket.   */
	/*************************************************************/

	rc = fcntl(listen_sd, F_SETFL, O_NONBLOCK);

	/*************************************************************/
	/* Bind the socket                                           */
	/*************************************************************/

	rc = bind_socket(rc, listen_sd);
	if (rc < 0)
		return (-1);
	/*************************************************************/
	/* Set the listen back log                                   */
	/*************************************************************/

	rc = listen(listen_sd, 32);
	if (rc < 0)
	{
	  perror("listen() failed");
	  close(listen_sd);
	  return (-1);
	}
	/*************************************************************/
	/* Initialize the pollfd structure                           */
	/*************************************************************/
	memset(events_var, 0 , sizeof(events_var));
	/*************************************************************/
	/* Set up the initial listening socket                        */
	/*************************************************************/
	events_var[0].data.fd = rc;
	events_var[0].events = EPOLLIN;
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
		/* Call poll() and wait 3 minutes for it to complete.      */
		/***********************************************************/
		rc = my_poll(rc, nfds);
		if (rc < 0)
			break ;
		/***********************************************************/
		/* One or more descriptors are readable.  Need to          */
		/* determine which ones they are.                          */
		/***********************************************************/
		std::cout << "current_size: " << current_size << std::endl;
		current_size = nfds;
		for (i = 0; i < current_size; i++)
		{
			/*********************************************************/
			/* Loop through to find the descriptors that returned    */
			/* POLLIN and determine whether it's the listening       */
			/* or the active connection.                             */
			/*********************************************************/
			if(events_var[i].events == 0)
			  continue;
			/*********************************************************/
			/* If revents is not POLLIN, it's an unexpected result,  */
			/* log and end the server.                               */
			/*********************************************************/
			if(events_var[i].events != EPOLLIN)
			{
			  printf("  Error! revents = %d\n", events_var[i].events);
			  end_server = TRUE;
			  break;
			}
			if (events_var[i].data.fd == listen_sd)
			{
				nfds = ft_iff(new_sd, listen_sd, end_server, nfds, events_var);
			}
			else
			{
				ft_ellse(close_conn, rc, len, buffer, i, events_var, compress_array);
			}
	}
	/* End of loop through pollable descriptors              */
	/***********************************************************/
	/* If the compress_array flag was turned on, we need       */
	/* to squeeze together the array and decrement the number  */
	/* of file descriptors. We do not need to move back the    */
	/* events and revents fields because the events will always*/
	/* be POLLIN in this case, and revents is output.          */
	/***********************************************************/

	/* End of serving running.    */
		ft_compress_array(compress_array, nfds, events_var, i, j);

	}	while (end_server == FALSE);

	ft_close(i, nfds, events_var);
}

//
//
//
//
//
//


