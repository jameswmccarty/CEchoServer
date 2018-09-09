/************************************************ 
 * Wrapper functions for TCP connection set up  *
 * Sections borrowed from TCP/IP Sockets in C   *
 ************************************************/

#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include "TCPSocketLib.h"

#define MAXPENDING 5

void DieWithUserMessage(const char *msg, const char *detail) {
	fputs(msg, stderr);
	fputs(":", stderr);
	fputs(detail, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DieWithSystemMessage(const char *msg) {
	perror(msg);
	exit(1);
}

int AcceptTCPConnection(int servSock, struct sockaddr *clientAddress) {
	struct sockaddr_in clntAddr; /* Client address */
	/* set length of client address structure (in-out parameter) */
	socklen_t clntAddrLen = sizeof(clntAddr);
	int clntSock;

	/* Wait for a client to connect */
	clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if(clntSock < 0)
		if(errno != EINTR)
			DieWithSystemMessage("accept() failed");
	   
	/* clntSock is connected to a client */

	/* copy out address info */
	memcpy(clientAddress, &clntAddr, sizeof(struct sockaddr_in));

	return clntSock;
}

int SetupTCPServerSocket(int service) {
	int servSock; /* socket descriptor for server */
	struct sockaddr_in servAddr; /* local address */

	/* Create socket for incoming connections */
	servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(servSock < 0)
		DieWithSystemMessage("socket() failed");
	
	/* Construct local address structure */
	memset(&servAddr, 0, sizeof(servAddr)); /* zero out structure */
	servAddr.sin_family = AF_INET;			/* IPv4 family*/
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	/* Any incoming interface */
	servAddr.sin_port = htons(service);	/* Local port */

	/* Bind to local address */
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("bind() failed");
	
	/* listen for incoming connections */
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed");
	
	return servSock;
}
