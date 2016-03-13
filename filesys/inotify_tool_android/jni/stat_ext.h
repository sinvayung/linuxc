#ifndef __STAT_EXT_H__
#define __STAT_EXT_H__

char *dirname_s(char *pathname);
char *basename_s(char *pathname);
void mkdirs(const char *pathname, mode_t mode);

#endif