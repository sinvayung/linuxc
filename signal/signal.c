/*
 * signal.c
 *
 *  Created on: 2015年2月8日
 *      Author: rongxinhua
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>  /* sleep() */
#include <stdlib.h>  /* exit() */

static void onHandler(int sig)
{
	printf("sig is %d\n", sig);
	exit(sig);
}

static void test_signal()
{
	if(signal(SIGINT, onHandler) == SIG_ERR)
		perror("signal error");
	int j;
	for(j = 0; ; j++)
	{
		printf("%d\n", j);
		sleep(3);
		if(j == 5) {
			raise(SIGINT);
		}
	}
}

int main()
{
	test_signal();
	return 0;
}
