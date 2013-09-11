#include "array.h"
#include <stdlib.h>

struct _array {
	int len, cap;
	void **els;
};

array *array_create(void)
{
	return (array *)calloc(1, sizeof(array));
}

int array_add(array *a, void *el)
{
	if(a->len == a->cap) {
		if(a->cap == 0)
			a->cap = 256;
		else
			a->cap <<= 1;
		a->els = realloc(a->els, a->cap * sizeof(void *));
	}

	a->els[a->len++] = el;

	return a->len - 1;
}

void *array_get(array *a, int idx)
{
	if(idx >= a->len)
		return NULL;
	return a->els[idx];
}
