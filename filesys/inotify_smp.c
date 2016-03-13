#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>

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

int main(int argc, char **argv) {
	if (argc < 2) {
		errExit("too less argv");
	}
	
	int inotifyFd = inotify_init();
	if (inotifyFd == -1) {
		errExit("inotify_init");
	}	
	const char *pathname = argv[1];
	uint32_t mask = IN_ALL_EVENTS;
	int watchFd = inotify_add_watch(inotifyFd, pathname, mask);
	if (watchFd == -1) {
		errExit("inotify_add_watch");
	}

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
			displayIntifyEvent(event);
			p += sizeof(struct inotify_event) + event->len;
		}
		printf("--------------------------\n");
	}
	
	exit(EXIT_SUCCESS);

}
