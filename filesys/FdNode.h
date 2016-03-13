
#ifndef __FD_NODE__
#define __FD_NODE__

struct FdNode {
	int fd;
	char path[256];
	struct FdNode *next;
};

void addFdNode(int fd, const char *path);
struct FdNode *getFdNodeByfd(int fd);

#endif