#include "FdNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FdNode *headNode;

struct FdNode *getFdNodeByfd(int fd) {
	struct FdNode *node = headNode;
	while(node != NULL) {
		if (node->fd == fd) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void addFdNode(int fd, const char *path) {
	struct FdNode *node = malloc(sizeof(struct FdNode));
	memset(node, '\0', sizeof(struct FdNode));
	node->fd = fd;
	strncpy(node->path, path, strlen(path));
	if (headNode) {
		node->next = headNode;
	}
	headNode = node;
}

int main_FdNode(int argc, char **argv) {
	headNode = malloc(sizeof(struct FdNode));
	int i;
	for(i = 0; i < 10; i ++ ) {
		char path[256];
		sprintf(path, "/aaa/bbb/%d", i);
		addFdNode(i, path);
	}
	struct FdNode *node = getFdNodeByfd(5);
	printf("%d = %s\n", node->fd, node->path);
	return 0;
}