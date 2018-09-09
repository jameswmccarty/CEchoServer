#include <stdio.h>	/* for printf() and fprintf() */
#include <sys/socket.h>	/* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	/* for sockaddr_in and inet_addr() */
#include <stdlib.h>	/* supports all sorts of functionality */
#include <unistd.h>	/* for close() */
#include <string.h>	/* support any string ops */
#include "TCPSocketLib.h"
	
#define BUFSIZE    1024 /* The message buffer size */

int main(int argc, char *argv[])
{

    int serverSock;	/* Server Socket */
    int clientSock;	/* Client Socket */
    in_port_t serverPort;	/* The Server Local Port */
    struct sockaddr_in changeClntAddr;	/* Client address */
	char msgbuf[BUFSIZE];	/* recieve buffer size */
	char clntName[INET_ADDRSTRLEN];	/* String to contain clients address */
	ssize_t numBytesRcvd;
	size_t toSend;
	ssize_t numBytesSent;

	serverPort = (argc == 2) ? atoi(argv[1]) : 8007; /* Server runs on port 7007 by default */

    /* Create new TCP Socket for incoming requests*/

	serverSock = SetupTCPServerSocket(serverPort);	

    /* Loop server forever*/
    while(1)
    {

		/* Accept incoming connection */
		clientSock = AcceptTCPConnection(serverSock, (struct sockaddr *) &changeClntAddr);

		/* Extract client data from the packet */
		if (inet_ntop(AF_INET, &changeClntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("Handling client %s/%d\n", clntName, ntohs(changeClntAddr.sin_port));
		else
			puts("Unable to get client address");
		
		/* zero out our storage for i/o */
		memset(&msgbuf, 0, sizeof(msgbuf));

		/* get the raw buffer from the client */
		numBytesRcvd = recv(clientSock, msgbuf, BUFSIZE, 0);
		if (numBytesRcvd < 0)
			DieWithSystemMessage("recv() failed");

		printf("Got input: %s\n", msgbuf);

		/* Return message to client */
		toSend = sizeof(msgbuf);
		numBytesSent = send(clientSock, msgbuf, toSend, 0);
		if (numBytesSent < 0)
			DieWithSystemMessage("send() failed");
		else if (toSend != numBytesSent)
			DieWithUserMessage("send()", "sent an unexpected number of bytes");

		close(clientSock);
    }
}
