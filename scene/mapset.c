#include "mapset.h"
#include <dlist.h>
#include <data.h>
#include <stdlib.h>

struct _mapset {
	size_t len, cap;
	dlist *maps;
};

struct _map {
	dnode *node;
	rect r;
	void *data;
	size_t datalen;
};

mapset *mapset_create(void)
{
	mapset *set;

	set = calloc(1, sizeof(mapset));
	set->maps = dlist_create();

	return set;
}

map *_mapset_add(mapset *set, rect *r, void *data, size_t datalen)
{
	map *m;

	m = calloc(1, sizeof(map));
	m->r = *r;
	m->data = malloc(datalen);
	memcpy(m->data, data, datalen);
	m->datalen = datalen;
	m->node = dlist_add_first(set->maps, m);

	return m;
}

typedef struct _overlap overlap;
struct _overlap {
	rect r;
	dnode *node;
};

static int overlap_next(iter *i, map **d)
{
	map *c;
	overlap *o;
	o = (overlap *)(i->data.v);
	while(o->node != NULL) {
		dnode_data(o->node, &c);
		if(rect_isoverlap(&(c->r), &(o->r)))
			break;
		o->node = dnode_next(o->node);
	}
	if(o->node == NULL) {
		free(o);
		return 0;
	}
	o->node = dnode_next(o->node);
	*d = c;

	return 1;
}

void mapset_iter_overlap(mapset *set, iter *i, rect *r)
{
	overlap *o;

	o = calloc(1, sizeof(overlap));
	i->next = (iter_fn)overlap_next;
	o->r = *r;
	o->node = dlist_first(set->maps);
	i->data = (data)(void *)o;
}

void map_setrect(map *m, rect *r)
{
	m->r = *r;
}

void _map_setdata(map *m, void *data, size_t datalen)
{
	free(m->data);
	m->data = malloc(datalen);
	memcpy(m->data, data, datalen);
	m->datalen = datalen;
}

rect *map_rect(map *m)
{
	return &(m->r);
}

void map_data(map *m, void *data)
{
	memcpy(data, m->data, m->datalen);
}

void map_destroy(map *m)
{
	dnode_rem(m->node);
	free(m);
}
