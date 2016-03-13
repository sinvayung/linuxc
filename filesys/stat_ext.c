#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "stat_ext.h"

char *dirname_s(char *pathname) {
	char *pathnameRight = strrchr(pathname, '/');
	*pathnameRight = '\0';
	return pathname;
}

char *basename_s(char *pathname) {
	char *pathnameRight = strrchr(pathname, '/');
	return pathnameRight + 1;
}

void mkdirs(const char *pathname, mode_t mode) {
	char *path = strdup(pathname);
	char *part = path + 1;	//skip first '/'
	while (part = strchr(part, '/')) {
		*part = '\0';
		if (access(path, F_OK)) {
			if (mkdir(path, mode)) {
				fprintf(stderr, "mkdirs fail: mkdir %s\n", path);
				return;
			}
		}
		*part = '/';
		part += 1;
	}
	free(path);
}

int main_stat_ext(int argc, char **argv) {
	if (argc < 2) {
		perror("too less argv");
		exit(-1);
	}
	char *path = argv[1];
	
	mkdirs(path, 0755);

	return 0;
}
