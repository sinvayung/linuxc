#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "FdNode.h"
#include "stat_ext.h"

#define errExit(MSG) {\
	perror(MSG);\
	exit(-1);\
	};

#define BUF_LEN 1024

static void displayIntifyEvent(struct inotify_event *ep) {
	printf("wd = %2d; ", ep->wd);
	printf("mask = ");
	if (ep->mask & IN_ACCESS)	printf("IN_ACCESS; ");
	if (ep->mask & IN_ATTRIB)	printf("IN_ATTRIB; ");
	if (ep->mask & IN_CLOSE_WRITE)	printf("IN_CLOSE_WRITE; ");
	if (ep->mask & IN_CLOSE_NOWRITE)	printf("IN_CLOSE_NOWRITE; ");
	if (ep->mask & IN_CREATE)	printf("IN_CREATE; ");
	if (ep->mask & IN_DELETE)	printf("IN_DELETE; ");
	if (ep->mask & IN_DELETE_SELF)	printf("IN_DELETE_SELF; ");
	if (ep->mask & IN_MODIFY)	printf("IN_MODIFY; ");
	if (ep->mask & IN_MOVE_SELF)	printf("IN_MOVE_SELF; ");
	if (ep->mask & IN_MOVED_FROM)	printf("IN_MOVE_FROM; ");
	if (ep->mask & IN_MOVED_TO)	printf("IN_MOVED_TO; ");
	if (ep->mask & IN_OPEN)	printf("IN_OPEN; ");
	if (ep->len > 0) {
		printf("name = %s; ", ep->name);
	}
	printf("\n");
}

static int inotifyFd;
uint32_t mask = IN_CREATE | IN_CLOSE_WRITE;

static char rootdirPath[PATH_MAX] = {0};
static char outdirPath[PATH_MAX] = {0};

static void copyFile(const char *srcPath, const char *destPath) {
	FILE *srcFp = fopen(srcPath, "rb");
	if (!srcFp) {
		fprintf(stderr, "copyFile fail: fopen %s\n", srcPath);
		return;
	}
	
	//Prepare
	mkdirs(destPath, 0777);
	
	FILE *destFp = fopen(destPath, "wb");
	if (!destFp) {
		fprintf(stderr, "copyFile fail: fopen %s\n", destPath);
		return;
	}
	
	//Copy file
 	char buff[BUF_LEN] = {0};
	size_t rs;
	while((rs = fread(buff, 1, BUF_LEN, srcFp)) > 0) {
		size_t ws = fwrite(buff, 1, rs, destFp);
		printf("rs = %d, ws = %d\n", rs, ws);
		if (rs != ws) {
			fprintf(stderr, "fwrite error: %s\n", destPath);
			break;
		}
	}
	printf("rs = %d\n", rs);
	fflush(destFp);
	fclose(srcFp);
	fclose(destFp);

	//set mode
	struct stat sb;
	stat(srcPath, &sb);
	chmod(destPath, sb.st_mode);

	chown(destPath, 2000, 2000);
}

static char *getCurrTimeStr(char *timeStr) {
	//time_t t;
	time_t t = time(NULL);
	//struct tm *m = gmtime(&t);
	struct tm *m = localtime(&t);
	sprintf(timeStr, "_%d_%d_%d", m->tm_hour, m->tm_min, m->tm_sec);
	return timeStr;
}

static void dumpFile(const char *filePath) {
	const char *relativePath = filePath + strlen(rootdirPath);
	char outFilePath[PATH_MAX] = {0};
	strncpy(outFilePath, outdirPath, strlen(outdirPath));
	strncat(outFilePath, relativePath, strlen(relativePath));
	char timeStr[10] = {0};
	getCurrTimeStr(timeStr);
	strncat(outFilePath, timeStr, strlen(timeStr));
	printf("outFilePath = %s\n", outFilePath);
	copyFile(filePath, outFilePath);
}

static void addWatch(const char *watchdir) {
	printf("addWatch: %s\n", watchdir);
	int watchFd = inotify_add_watch(inotifyFd, watchdir, mask);
	if (watchFd == -1) {
		errExit("inotify_add_watch");
	}
	
	addFdNode(watchFd, watchdir);
	
	int numRead;
	char buf[BUF_LEN];
	char *p;
	struct inotify_event *event;
	for (;;) {
		numRead = read(inotifyFd, buf, BUF_LEN);
		if (numRead == 0) {
			fprintf(stderr, "read() from intofiy fd returned 0!");		
		}
		if (numRead == -1) {
			errExit("read");
		}
		for (p = buf; p < buf + numRead; ) {
			event = (struct inotify_event *)p;
			
			struct FdNode *fn = getFdNodeByfd(event->wd);
			char *path = fn->path;
			
			if (event->mask & IN_CLOSE_WRITE) {
				char filename[PATH_MAX] = {0};
				sprintf(filename, "%s/%s", fn->path, event->name);
				printf("IN_CLOSE_WRITE: %d_%s\n", fn->fd, filename);
				dumpFile(filename);
			}
			if (event->mask & IN_CREATE) {
				char filename[PATH_MAX] = {0};
				sprintf(filename, "%s/%s", fn->path, event->name);
				printf("IN_CREATE: %d_%s\n", fn->fd, filename);
				struct stat stbuf;
				stat(filename, &stbuf);
				if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
					printf("addWatch: %s\n", filename);
					watchFd = inotify_add_watch(inotifyFd, filename, mask);
					if (watchFd == -1) {
						errExit("inotify_add_watch");
					}
					addFdNode(watchFd, filename);
				}
				
			}
			
			p += sizeof(struct inotify_event) + event->len;
		}
		printf("--------------------------\n");
	}
}

static void startWatch(const char *rootdir, const char *outdir){
	realpath(rootdir, rootdirPath);
	realpath(outdir, outdirPath);
	printf("rootdirPath = %s\nrootdirPath = %s\n", rootdirPath, outdirPath);
	
	inotifyFd = inotify_init();
	if (inotifyFd == -1) {
		errExit("inotify_init");
	}
	addWatch(rootdirPath);
}

int main(int argc, char **argv) {
	//copyFile("/home/rongxinhua/test/aaa", "/home/rongxinhua/test/dd/bbb");
	
	if (argc < 3) {
		errExit("too less argv");
	}
	const char *rootdir = argv[1];
	const char *outdir = argv[2];
	
	printf("rootdir = %s\n", rootdir);
	printf("outdir = %s\n", outdir);
	
	startWatch(rootdir, outdir);
	
	exit(EXIT_SUCCESS);
}
