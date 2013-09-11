#ifndef ARRAY_H
#define ARRAY_H

typedef struct _array array;

array *array_create(void);
int array_add(array *a, void *el);
void *array_set(array *a, void *el);
void *array_get(array *a, int idx);

#endif
