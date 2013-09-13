#ifndef DLIST_H
#define DLIST_H

/* doubly linked list */

#include <data.h>
#include <iter.h>

typedef struct _dlist dlist;
typedef struct _dnode dnode;

dlist *dlist_create(void);

data dnode_data(dnode *dl);
dnode *dnode_next(dnode *dn);
dnode *dnode_prev(dnode *dn);

dnode *dlist_last(dlist *dl); /* end of a next chain */
dnode *dlist_first(dlist *dl); /* end of a prev chain */

data dnode_rem(dnode *dn);
dnode *dlist_add_last(dlist *dl, data data);
dnode *dlist_add_first(dlist *dl, data data);

int dlist_iter(dlist *dl, iter *i);

#endif
