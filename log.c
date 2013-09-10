#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"
#include "main.h"

static void vwarning(const char *fmt, va_list ap);
static char errmsg[256] = { 0 };

char *geterrmsg(void)
{
	return errmsg;
}

int seterrmsg(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(errmsg, sizeof(errmsg), fmt, ap);
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
	fail();
}

void warning(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vwarning(fmt, ap);
	va_end(ap);
}
