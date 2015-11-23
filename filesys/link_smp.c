/*
 * link.c
 *
 *  Created on: 2015年2月8日
 *      Author: rongxinhua
 */

#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>	/* dirname(),basename() */
#include <unistd.h>
#include <errno.h>
#include <limits.h> /* PATH_MAX */
#include <stdlib.h> /* realpath() */
#include <fcntl.h>	/* O_RDONLY, O_WRONLY */

static void test_link()
{
	char cwdbuf[PATH_MAX] = {0};
	char *cwdpath = getcwd(cwdbuf, sizeof(cwdbuf) - 1);
	if(cwdpath == NULL)
		fprintf(stderr, "getcwd fail. errno = %d\n", errno);

	char linkpath[PATH_MAX], linkpath2[PATH_MAX];
	sprintf(linkpath, "%s/mycat", cwdpath);
	sprintf(linkpath2, "%s/mycat2", cwdpath);
	const char *srcpath = "/bin/cat";
	symlink(srcpath, linkpath);
	link(srcpath, linkpath2);


	char destpath[PATH_MAX] = {0};
	readlink(linkpath, destpath, sizeof(destpath) - 1);
	printf("getcwd = %s\nlinkpath = %s\ndestpath = %s\n", cwdpath, linkpath, destpath);

	char resolved_path[PATH_MAX] = {0};
	realpath(linkpath, resolved_path);
	printf("dirname = %s\nbasename = %s\nrealpath = %s\n", dirname(linkpath), basename(linkpath), resolved_path);

}

int main(int argc, char **argv) {
	test_link();
}

