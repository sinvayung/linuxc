/*
 * unix_stream_client.c
 *
 *  Created on: Apr 29, 2015
 *      Author: rongxinhua
 */

#include "unix_socket.h"

int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int sfd;
	ssize_t numRead;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sfd == -1)
		errExit("socket");

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1);

	if(connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
		errExit("connect");

	/* Copy stdin to socket */
	while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
		if(write(sfd, buf, numRead) != numRead)
			perror("partial/failed write");

	if(numRead == -1)
		errExit("read");

	exit(EXIT_SUCCESS);
}
