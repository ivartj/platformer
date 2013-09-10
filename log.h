#ifndef LOG_H
#define LOG_H

char *geterrmsg(void);
int seterrmsg(const char *fmt, ...);
void error(const char *fmt, ...);
void warning(const char *fmt, ...);

#endif
