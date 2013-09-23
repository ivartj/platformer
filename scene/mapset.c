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
	data data;
};

mapset *mapset_create(void)
{
	mapset *set;

	set = calloc(1, sizeof(mapset));
	set->maps = dlist_create();

	return set;
}

map *mapset_add(mapset *set, rect *r, data d)
{
	map *m;

	m = calloc(1, sizeof(map));
	m->r = *r;
	memcpy(&(m->data), &d, sizeof(d)); /* syntax error when using equal sign */
	m->node = dlist_add_first(set->maps, (data)(void *)m);

	return m;
}

typedef struct _overlap overlap;
struct _overlap {
	rect r;
	dnode *node;
};

static int overlap_next(iter *i, data *d)
{
	map *c;
	overlap *o;
	o = (overlap *)(i->data.v);
	while(o->node != NULL) {
		c = (map *)(dnode_data(o->node).v);
		if(rect_isoverlap(&(c->r), &(o->r)))
			break;
		o->node = dnode_next(o->node);
	}
	if(o->node == NULL) {
		free(o);
		return 0;
	}
	o->node = dnode_next(o->node);
	*d = (data)(void *)c;

	return 1;
}

void mapset_iter_overlap(mapset *set, iter *i, rect *r)
{
	overlap *o;

	o = calloc(1, sizeof(overlap));
	i->next = overlap_next;
	o->r = *r;
	o->node = dlist_first(set->maps);
	i->data = (data)(void *)o;
}

void map_setrect(map *m, rect *r)
{
	m->r = *r;
}

void map_setdata(map *m, data data)
{
	m->data = data;
}

rect *map_rect(map *m)
{
	return &(m->r);
}

data map_data(map *m)
{
	return m->data;
}

void map_destroy(map *m)
{
	dnode_rem(m->node);
	free(m);
}
