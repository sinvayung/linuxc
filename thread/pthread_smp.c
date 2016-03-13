#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define errExit(MSG) {\
	perror(MSG);\
	exit(-1);\
	};

static void *threadFunc(void *arg) {
	char *s = (char *)arg;
	pthread_t childThread = pthread_self();
	printf("childThread: %d, arg: %s\n", (int)childThread, s);
	sleep(2);
	return (void *)strlen(s);
}

int main() {
	pthread_t t1;
	void *res;
	int s;
	s = pthread_create(&t1, NULL, threadFunc, "Hello World\n");
	if (s != 0)
		errExit("pthread_create");
	printf("Message from main()\n");
	s = pthread_join(t1, &res);
	if (s != 0) 
		errExit("pthread_join");
	printf("Thread returned %ld\n", (long)res);
	exit(EXIT_SUCCESS);
}
