/*
 * time.c
 *
 *  Created on: 2015年2月8日
 *      Author: rongxinhua
 */

#include <stdio.h>
#include <time.h>

int main()
{
	time_t t = time(NULL);
	printf("time is %ld\n", t);		//1423389909

	char *ts = ctime(&t);
	printf("time is %s", ts);		//Sun Feb  8 18:05:09 2015

	//struct tm *tms = gmtime(&t);	//格林时间
	struct tm *tms = localtime(&t);
	printf("%d-%d-%d %d:%d:%d\n",
		tms->tm_year + 1900,
		tms->tm_mon + 1,
		tms->tm_mday + 1,
		tms->tm_hour,
		tms->tm_min,
		tms->tm_sec);		//2015-2-9 18:5:9

	time_t tt = mktime(tms);
	printf("tt = %ld\n", tt);		//1423389909

	return 0;
}
