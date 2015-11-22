/*
 * unix_datagram_client.c
 *
 *  Created on: Apr 29, 2015
 *      Author: rongxinhua
 */

#include "unix_socket.h"

int main(int argc, char *argv[])
{
	struct sockaddr_un svaddr, claddr;
	int sfd;
	ssize_t numBytes;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sfd == -1)
		errExit("socket");

	memset(&claddr, 0, sizeof(struct sockaddr_un));
	claddr.sun_family = AF_UNIX;
	snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/us_client_path.%ld", (long)getpid());

	memset(&svaddr, 0, sizeof(struct sockaddr_un));
	svaddr.sun_family = AF_UNIX;
	strncpy(svaddr.sun_path, SOCK_PATH, sizeof(svaddr.sun_path) - 1);

	if(bind(sfd, (struct sockaddr *)&claddr, sizeof(struct sockaddr_un)) == -1)
		errExit("bind");

	/* Copy stdin to socket */
	while((numBytes = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
		if(sendto(sfd, buf, numBytes, 0, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un)) != numBytes)
			perror("sendto");

	exit(EXIT_SUCCESS);
}
