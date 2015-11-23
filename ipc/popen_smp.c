/*
 * popen.c
 *
 *  Created on: 2015年2月8日
 *      Author: rongxinhua
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define PIPE_BUF_SIZE 1024

static void test_popen()
{
	FILE *fp = popen("ls -l", "r");
	if(fp)
	{
		char *buf = (char *)calloc(1, PIPE_BUF_SIZE + 1);
		while(fgets(buf, PIPE_BUF_SIZE, fp) != NULL)
		{
			int buflen = strlen(buf);
			printf("line : %s\n", buf);
		}
		pclose(fp);
		free(buf);
	}
}

int main(int argc, char **argv) {
	test_popen();
	return 0;
}

