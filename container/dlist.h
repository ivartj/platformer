#ifndef DLIST_H
#define DLIST_H

/* doubly linked list */

#include <data.h>
#include <iter.h>

typedef struct _dlist dlist;
typedef struct _dnode dnode;

dlist *dlist_create(void);

#define dnode_data(dl, data) _dnode_data(dl, data, sizeof(*data))
void _dnode_data(dnode *dl, void *data, size_t datalen);
dnode *dnode_next(dnode *dn);
dnode *dnode_prev(dnode *dn);

dnode *dlist_last(dlist *dl); /* end of a next chain */
dnode *dlist_first(dlist *dl); /* end of a prev chain */

void dnode_rem(dnode *dn);

#define dlist_add_last(dl, data) _dlist_add_last(dl, &data, sizeof(data))
dnode *_dlist_add_last(dlist *dl, void *data, size_t datalen);

#define dlist_add_first(dl, data) _dlist_add_first(dl, &data, sizeof(data))
dnode *_dlist_add_first(dlist *dl, void *data, size_t datalen);

int dlist_iter(dlist *dl, iter *i);

#endif
