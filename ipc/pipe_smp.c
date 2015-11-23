#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * PIPE
 */
static void test_pipe() {

	int filedes[2];
	pid_t pid;

	int data_processed = 0;
	const char data[] = "Hello pipe!";
	char buffer[BUFSIZ + 1];
	//清空缓冲区
	memset(buffer, '\0', sizeof(buffer));

	printf("my.pid = %d\n", getpid());

	if (pipe(filedes) == 0)
	{
		//创建管道成功
		//通过调用fork创建子进程
		pid = fork();
		if(pid == -1) 
		{
			fprintf(stderr, "Fork failure");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			//子进程中
			printf("child.pid = %d, parent.pid = %d\n", getpid(), getppid());
			//读取数据
			data_processed = read(filedes[0], buffer, BUFSIZ);
			printf("child.Read %d bytes : %s\n", data_processed, buffer);
			exit(EXIT_SUCCESS);
		} 
		else 
		{
			//父进程中
			printf("my.pid = %d, child.pid = %d\n", getpid(), pid);
			//写数据
			data_processed = write(filedes[1], data, strlen(data));
			printf("my.Wrote %d bytes: %s\n", data_processed, data);
			exit(EXIT_SUCCESS);
		}
	}
}

int main(int argc, char **argv) {
	test_pipe();
}

