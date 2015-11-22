/*
 * unix_datagram_server.c
 *
 *  Created on: Apr 29, 2015
 *      Author: rongxinhua
 */

#include "unix_socket.h"


int main(int argc, char *argv[])
{
	struct sockaddr_un svaddr, claddr;
	int sfd, cfd;
	char buf[BUF_SIZE];
	socklen_t len;
	ssize_t numBytes;

	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sfd == -1)
		errExit("socket");
	if(remove(SOCK_PATH) == -1 && errno != ENOENT)
		errExit("remove-sockpath");

	memset(&svaddr, 0, sizeof(struct sockaddr_un));
	svaddr.sun_family = AF_UNIX;
	strncpy(svaddr.sun_path, SOCK_PATH, sizeof(svaddr.sun_path) - 1);
	if(bind(sfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un)) == -1)
		errExit("bind");

	for(;;){
		len = sizeof(struct sockaddr_un);
		numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&claddr, &len);
		if(numBytes == -1)
			errExit("recvfrom");

		printf("Server received %ld bytes from %s\n", (long)numBytes, claddr.sun_path);

		if(write(STDOUT_FILENO, buf, numBytes) != numBytes)
			perror("partial/failed write");
	}
}

