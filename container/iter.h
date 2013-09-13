#ifndef ITER_H
#define ITER_H

/* iterator interface */

#include <data.h>

typedef struct _iter iter;

struct _iter {
	data data;
	int (*next)(iter *i, data *retval);
};

int iterate(iter *i, data *retval);

#endif
