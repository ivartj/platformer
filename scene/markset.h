#ifndef MARKSET_H
#define MARKSET_H

#include <rect.h>
#include <iter.h>

typedef struct _markset markset;

markset *markset_create(void);
void markset_add(markset *set, rect *r);
void markset_clear(markset *set);
int markset_iter(markset *set, iter *i);

#endif
