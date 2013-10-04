#ifndef ITER_H
#define ITER_H

/* iterator interface */

#include <data.h>

typedef struct _iter iter;
typedef int (*iter_fn)(iter *i, void *retval);

struct _iter {
	data data;
	iter_fn next;
};

int iterate(iter *i, void *retval);

#endif
