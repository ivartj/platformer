#ifndef DLIST_H
#define DLIST_H

/* doubly linked list */

typedef struct _dlist dlist;
typedef struct _dnode dnode;

dlist *dlist_create(void);

void *dnode_data(dnode *dl);
dnode *dnode_next(dnode *dn);
dnode *dnode_prev(dnode *dn);

dnode *dlist_last(dlist *dl); /* end of a next chain */
dnode *dlist_first(dlist *dl); /* end of a prev chain */

void *dnode_rem(dnode *dn);
dnode *dlist_add_last(dlist *dl, void *data);
dnode *dlist_add_first(dlist *dl, void *data);

#endif
