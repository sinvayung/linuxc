/*
 * readdir.c
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
//#include <sys/inotify.h>
#define _XOPEN_SOURCE 600  /*Get nftw() and S_IFSOCK declarations*/
#include <ftw.h>

#define BUF_SIZE PATH_MAX


static void printStat(struct stat *stp)
{
	printf("uid = %d, gid = %d, mode = %d, size = %lld, mtime = %ld\n",
		stp->st_uid, stp->st_gid, stp->st_mode, stp->st_size, stp->st_mtime);
	if ((stp->st_mode & S_IFMT) == S_IFLNK) {
		printf("Is Link");
		/* S_IFREG, S_IFDIR, S_IFCHR, S_IFBLK, S_IFIFO, S_IFSOCK, S_IFLNK */
	}
}

static void test_traverse(const char *dirpath)
{
	printf("IS DIR...\n");
	DIR *dirp = opendir(dirpath);
	if (dirp == NULL) {
		fprintf(stderr, "opendir failed on '%s'\n", dirpath);
		return;
	}
	struct dirent *dp;
	do {
		dp = readdir(dirp);
		if(dp == NULL)
			break;
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;		/* Skip . and .. */
		char path[256];
		sprintf(path, "%s/%s", dirpath, dp->d_name);

		struct stat st;
		if (lstat(path, &st))
			perror("lstat fail.\n");
		printStat(&st);
		if((st.st_mode & S_IFMT) == S_IFDIR)
		{
			printf(" + %s\n", path);
			test_traverse(path);
		}
		else
		{
			printf(" - %s\n", path);
		}

	} while(1);
	if (closedir(dirp) == -1)
		perror("closedir fail.");
}

/**
 *
 * @param pathname
 * @param sbuf
 * @param typeflag
 * @param ftwb
 */
static int dirTree(const char *pathname, const struct stat *sbuf, int typeflag, struct FTW *ftwb)
{
	printf("%s : %d, %d:%s, %d, %s\n",
		pathname,
		(sbuf->st_mode & S_IFMT),
		typeflag,
		typeflag == FTW_D ? "D" : (typeflag == FTW_F ? "F" : ""),
		ftwb->level,
		&pathname[ftwb->base]);
	return 0;
}

static void test_nftw(const char *dirpath)
{
	if(nftw(dirpath, dirTree, 10, FTW_DEPTH|FTW_PHYS) == -1)
	{
		perror("nftw error");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "Parameter error.\n");
		return -1;
	}
	const char *dirpath = "";
	if(strcmp(argv[1], "1"))
		test_traverse(dirpath);
	else
		test_nftw(dirpath);
}


