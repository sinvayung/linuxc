#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * FIFO write
 */
static void test_fifo_write(const char *fifo_path)
{
	int pipe_fd = -1;
	int data_fd = -1;
	int res = 0;
	const int open_mode = O_WRONLY;
	int bytes_sent = 0;
	char buffer[PIPE_BUF + 1];
	if (access(fifo_path, F_OK) == -1)
	{
		//管道文件不存在
		//创建命名管道
		res = mkfifo(fifo_path, 0777);
		if (res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", fifo_path);
			exit(EXIT_FAILURE);
		}
	}
	printf("Process %d opening FIFO O_WRONLY\n", getpid());
	//以只写阻塞方式打开FIFO文件，以只读方式打开数据文件
	pipe_fd = open(fifo_path, open_mode);
	data_fd = open("Data.txt", O_RDONLY);
	if(data_fd == -1) 
	{
		fprintf(stderr, "Data.txt is not exists.\n");
		exit(EXIT_FAILURE);
	}
	printf("Process %d result %d\n", getpid(), pipe_fd);
	if(pipe_fd != -1) 
	{
		int bytes_read = 0;
		//向数据文件读取数据
		bytes_read = read(data_fd, buffer, PIPE_BUF);
		buffer[bytes_read] = '\0';
		while(bytes_read > 0) 
		{
			//向FIFO文件写数据
			res = write(pipe_fd, buffer, bytes_read);
			if(res == -1) 
			{
				fprintf(stderr, "Write error on pipe\n");
				exit(EXIT_FAILURE);
			}
			//累加写的字节数，并继续读取数据
			bytes_sent += res;
			bytes_read = read(data_fd, buffer, PIPE_BUF);
			buffer[bytes_read] = '\0';
		}
		close(pipe_fd);
		close(data_fd);
	}
	else 
		exit(EXIT_FAILURE);
	printf("Proccess %d finished\n", getpid());
	exit(EXIT_SUCCESS);
}

/**
 * FIFO read
 */
static int test_fifo_read(const char *fifo_path)
{
	int pipe_fd = -1;
	int data_fd = -1;
	int res = 0;
	int open_mode = O_RDONLY;
	char buffer[PIPE_BUF + 1];
	int bytes_read = 0;
	int bytes_write = 0;
	//清空缓冲数组
	memset(buffer, '\0', sizeof(buffer));

	printf("Process %d opening FIFO O_RDONLY\n", getpid());
	//以只读阻塞方式打开管道文件， 注意与fifowrite_c文件中的FIFO同名
	pipe_fd = open(fifo_path, open_mode);
	//以只写方式创建保存数据的文件
	data_fd = open("DataFromFIFO.txt", O_WRONLY|O_CREAT, 0644);
	printf("Process %d result %d\n", getpid(), pipe_fd);

	if (pipe_fd != -1)
	{
		do
		{
			res = read(pipe_fd, buffer, PIPE_BUF);
			bytes_write = write(data_fd, buffer, res);
			bytes_read += res;
		} while(res > 0);
		close(pipe_fd);
		close(data_fd);
	}
	else
		exit(EXIT_FAILURE);
	printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "Parameter error.\n");
		return -1;
	}
	const char *fifo_path = "/tmp/my_fifo";
	if(strcmp(argv[1], "-r") == 0)
		test_fifo_read(fifo_path);
	else if(strcmp(argv[1], "-w") == 0)
		test_fifo_write(fifo_path);
	return 0;
}

