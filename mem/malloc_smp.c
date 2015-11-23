/*
 * malloc.c
 *
 *  Created on: 2015年3月1日
 *      Author: rongxinhua
 */
#include <stdio.h>
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
	
	struct X *y = x + 1;
	y -> a = 6;
	y -> b = "aaaa";
	
	printf("sizeof x = %lu\n", sizeof(x));
	printf("%p = {%d, %s}\n%p = {%d, %s}\n", x, x->a, x->b, y, y->a, y->b);
	
	free(x);
	//free(y);	//Error: double free or corruption (out)
}

int main(int argc, char **argv) {
	test_malloc();
	test_calloc();
	return 0;
}



