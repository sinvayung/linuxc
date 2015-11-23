/*
 * log.c
 *
 *  Created on: 2015年2月8日
 *      Author: rongxinhua
 */

#include <stdio.h>	/*va_list*/
#include <stdarg.h>	/*va_start(), va_end()*/
#include <stdlib.h>

/*__VA_ARGS__被替换为函数宏定义中的"..."*/
#define my_printf(...) fprintf(stdout, __VA_ARGS__)

static void logger_printf(const char *fmt, ...)
{
	char logBuffer[512] = {0};
	va_list args;
	va_start(args, fmt);
	vsnprintf(logBuffer, sizeof(logBuffer) - 1, fmt, args);
	va_end(args);

	fputs(logBuffer, stdout);
	fflush(stdout);
}

static void test_log_fmt()
{
	logger_printf("test log %s, %d.\n", "abc", 123);
}

static void test_log_func()
{
	printf("%s, %d, %s\n", __FILE__, __LINE__, __FUNCTION__);
}

#define LOG_FUNC(...) printf("%s, %d, %s, "#__VA_ARGS__"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

static void test_log_func_args()
{
	LOG_FUNC("abc", 123);	//log.c, 41, test_log_func_args, "abc", 123
}

int main(int argc, char **argv) {
	test_log_fmt();
	test_log_func();
	test_log_func_args();
	return 0;
}
