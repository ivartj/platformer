#include "dlist.h"
#include <log.h>

struct _dnode {
	void *data;
	size_t datalen;
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
	dlist *dl;

	dl = calloc(1, sizeof(dlist));

	return dl;
}

void _dnode_data(dnode *dn, void *data, size_t datalen)
{
	if(datalen != dn->datalen)
		warning("dnode_data: mismatch between data type sizes stored and requested");
	if(datalen > dn->datalen)
		datalen = dn->datalen;
	memcpy(data, dn->data, datalen);
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

void dnode_rem(dnode *dn)
{
	if(dn->prev != NULL)
		dn->prev->next = dn->next;
	else
		dn->list->first = dn->next;
	if(dn->next != NULL)
		dn->next->prev = dn->prev;
	else
		dn->list->last = dn->prev;
	free(dn->data);
	free(dn);
}

dnode *_dlist_add_last(dlist *dl, void *data, size_t datalen)
{
	dnode *dn;

	dn = calloc(1, sizeof(dnode));
	dn->data = malloc(datalen);
	memcpy(dn->data, data, datalen);
	dn->datalen = datalen;
	dn->list = dl;
	if(dl->last != NULL) {
		dl->last->next = dn;
		dn->prev = dl->last;
	} else
		dl->first = dn;
	dl->last = dn;
	return dn;
}

dnode *_dlist_add_first(dlist *dl, void *data, size_t datalen)
{
	dnode *dn;

	dn = calloc(1, sizeof(dnode));
	dn->data = malloc(datalen);
	memcpy(dn->data, data, datalen);
	dn->datalen = datalen;
	dn->list = dl;
	if(dl->first != NULL) {
		dl->first->prev = dn;
		dn->next = dl->first;
	} else
		dl->last = dn;
	dl->first = dn;
	return dn;
}

static int dnode_iter_next(iter *i, void *retval);

int dlist_iter(dlist *dl, iter *i)
{
	i->data.v = dl->first;
	i->next = dnode_iter_next;
	return dl->first != NULL;
}

int dnode_iter_next(iter *i, void *retval)
{
	dnode *dn;
	dn = (dnode *)(i->data.v);
	if(dn == NULL)
		return 0;
	i->data.v = dn->next;
	memcpy(retval, dn->data, dn->datalen);
	return 1;
}
