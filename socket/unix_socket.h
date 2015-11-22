/*
 * unix_socket.h
 *
 *  Created on: Apr 29, 2015
 *      Author: rongxinhua
 */

#ifndef UNIX_SOCKET_H_
#define UNIX_SOCKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	/*STDOUT_FILENO*/
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCK_PATH "/tmp/us_server_path"
#define BUF_SIZE 10

#define errExit(str) \
	{ \
		perror(str); \
		exit(EXIT_FAILURE); \
	}

#endif /* UNIX_SOCKET_H_ */
