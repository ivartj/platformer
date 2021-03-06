#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"

static void vwarning(const char *fmt, va_list ap);
static char errmsg[256] = { 0 };

char *geterrmsg(void)
{
	return errmsg;
}

int seterrmsg(const char *fmt, ...)
{
	va_list ap;
	int len;
	/* vsnprintf with overlapping strings is undefined
	 * so we have a bakmsg to write before errmsg */
	char bakmsg[sizeof(errmsg)] = { 0 };

	va_start(ap, fmt);
	len = vsnprintf(bakmsg, sizeof(errmsg), fmt, ap);
	memcpy(errmsg, bakmsg, len + 1);
	va_end(ap);

	return -1;
}

void vwarning(const char *fmt, va_list ap)
{
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
}

void error(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vwarning(fmt, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

void warning(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vwarning(fmt, ap);
	va_end(ap);
}
