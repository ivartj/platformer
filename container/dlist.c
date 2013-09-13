#include "dlist.h"

struct _dnode {
	void *data;
	dnode *next;
	dnode *prev;
	dlist *list;
};

struct _dlist {
	dnode *first;
	dnode *last;
};

dlist *dlist_create(void)
{
	return (dlist *)calloc(1, sizeof(dlist));
}

void *dnode_data(dnode *dn)
{
	return dn->data;
}

dnode *dnode_next(dnode *dn)
{
	return dn->next;
}

dnode *dnode_prev(dnode *dn)
{
	return dn->prev;
}

dnode *dlist_last(dlist *dl)
{
	return dl->last;
}

dnode *dlist_first(dlist *dl)
{
	return dl->first;
}

void *dnode_rem(dnode *dn)
{
	void *data;

	data = dn->data;

	if(dn->prev != NULL)
		dn->prev->next = dn->next;
	else
		dn->list->first = dn->next;
	if(dn->next != NULL)
		dn->next->prev = dn->prev;
	else
		dn->list->last = dn->prev;
	free(dn);

	return data;
}

dnode *dlist_add_last(dlist *dl, void *data)
{
	dnode *dn;

	dn = calloc(1, sizeof(dnode));
	dn->data = data;
	dn->list = dl;
	if(dl->last != NULL) {
		dl->last->next = dn;
		dn->prev = dl->last;
	} else
		dl->first = dn;
	dl->last = dn;
	return dn;
}

dnode *dlist_add_first(dlist *dl, void *data)
{
	dnode *dn;

	dn = calloc(1, sizeof(dnode));
	dn->data = data;
	dn->list = dl;
	if(dl->first != NULL) {
		dl->first->prev = dn;
		dn->next = dl->first;
	} else
		dl->last = dn;
	dl->first = dn;
	return dn;
}
