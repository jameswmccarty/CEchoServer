
/* Included libraries */

#include <stdio.h>        /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "TCPSocketLib.h"

/* Constants */
#define BUFSIZE 1024	        /* The send/receive buffer size */

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;	/* socket descriptor */
    struct sockaddr_in serv_addr;	/* The server address */
	in_port_t serv_port;	/* Port to try on server */

    char *message;	/* message to send to echo server */
	char *serv_ip;	/* Address of Server */

    char sndbuf[BUFSIZE] = {0};	/* Send Buffer */
    char rcvbuf[BUFSIZE] = {0};	/* Receive Buffer */

	int counter;	/* counter to copy input to buffer */
	int rtnVal = 0;
	ssize_t numBytes;

    /* Get the message from the command line */
    if (argc < 3 || argc > 4)
    {
		printf("Incorrect input format. The correct format is:\n\tclient 'your_text' <Server Address> <Server Port>(Optional)\n");
		exit(1);
    }
    message = argv[1];
	serv_ip = argv[2];
	serv_port = (argc == 4) ? (atoi(argv[3])) : 8007; /* if not given, use port 8007 */

    /* Create a new TCP socket*/

	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(clientSock < 0)
		DieWithSystemMessage("socket() failed");

    /* Construct the server address structure */

	memset(&serv_addr, 0, sizeof(serv_addr)); /* Zero out structure */
	serv_addr.sin_family = AF_INET;	/* IPv4 address family */
	/* convert the address */
	rtnVal = inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr.s_addr);
	if (rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if (rtnVal < 0)
		DieWithSystemMessage("inet_pton() failed");
	serv_addr.sin_port = htons(serv_port); /* set serve port */

    /* Establish connection to the server */

	if (connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		DieWithSystemMessage("connect() failed");

    /* Send the string to the server */

	counter = 0;
	while (counter < BUFSIZE-1 && message[counter] != '\0' ) /* copy contents of string to sndbuffer */
	{
		sndbuf[counter] = message[counter];
		counter++;
	}

	numBytes = send(clientSock, sndbuf, counter, 0); /* send the buffer to the server */
	if(numBytes < 0)
		DieWithSystemMessage("send() failed");
	else if (numBytes != counter)
		DieWithUserMessage("send()", "sent unexpected number of bytes");

	printf("Sent: %s\n", message);

	/* Receive and print response from the server */
	printf("Received: ");

	numBytes = recv(clientSock, rcvbuf, BUFSIZE, 0);
	if (numBytes < 0)
		DieWithSystemMessage("recv() failed");
	else if (numBytes == 0)
		DieWithUserMessage("recv()", "connection closed prematurely");
	fputs(rcvbuf, stdout);
	fputc('\n', stdout);

	close(clientSock);
    return 0;
}

