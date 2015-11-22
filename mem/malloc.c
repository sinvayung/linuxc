/*
 * malloc.c
 *
 *  Created on: 2015年3月1日
 *      Author: rongxinhua
 */
#include <stdlib.h>

struct X {
	int a;
	char *b;
};

void test_malloc() {
	struct X *x;
	x = malloc(sizeof(struct X));
	if(!x) {
		perror("malloc error");
		exit(-1);
	}
	x -> a = 5;
	x -> b = "abcdefg";
	free(x);
}

void test_calloc() {
	struct X *x;
	x = calloc(2, sizeof(struct X));
	if(!x) {
		perror("malloc error");
		exit(-1);
	}
	x -> a = 5;
	x -> b = "abcdefg";
//	x += 1;
//	x -> a = 6;
//	x -> b = "aaaa";
	free(x);
}

int main(int argc, char **argv) {
	test_calloc();
	return 0;
}



