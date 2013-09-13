#include "markset.h"
#include <dlist.h>

struct _markset {
	dlist *dl;
};

static void split(markset *set, rect *a, rect *c);

markset *markset_create(void)
{
	markset *set;

	set = calloc(1, sizeof(markset));
	set->dl = dlist_create();

	return set;
}

void markset_add(markset *set, rect *r)
{
	rect *m;
	dnode *dn;

	if(r->w == 0 || r->h == 0)
		return;

	for(dn = dlist_first(set->dl); dn != NULL; dn = dnode_next(dn)) {
		m = (rect *)(dnode_data(dn).v);
		if(rect_iswithin(m, r))
			return;
	}
	for(dn = dlist_first(set->dl); dn != NULL; dn = dnode_next(dn)) {
		m = (rect *)(dnode_data(dn).v);
		if(rect_isoverlap(m, r)) {
			dnode_rem(dn);
			split(set, m, r);
			free(m);
			return;
		}
	}

	m = calloc(1, sizeof(rect));
	*m = *r;
	dlist_add_first(set->dl, (data)(void*)m);
}

void split(markset *set, rect *a, rect *c)
{
	rect *t, *tm;
	rect *b, *bm;
	rect nt, nm, nb;

	if(a->y < c->y) {
		t = a;
		tm = c;
	} else {
		t = c;
		tm = a;
	}

	if(a->y + a->h > c->y + c->h) {
		b = a;
		bm = c;
	} else {
		b = c;
		bm = a;
	}

	nt.x = t->x;
	nt.y = t->y;
	nt.w = t->w;
	nt.h = tm->y - t->y;

	nb.x = b->x;
	nb.y = bm->y + bm->h;
	nb.w = b->w;
	nb.h = b->y + b->h - nb.y;

	nm.y = tm->y;
	nm.h = nb.y - nm.y;
	nm.x = a->x < c->x ? a->x : c->x;
	nm.w = (a->x + a->w > c->x + c->w ? a->x + a->w : c->x + c->w) - nm.x;

	if(nt.w == nm.w && nt.x == nm.x) {
		nm.h += nt.h;
		nm.y = nt.y;
		nt.w = 0;
	}
	if(nm.w == nb.w && nm.x == nb.x) {
		nm.h += nb.h;
		nb.w = 0;
	}

	markset_add(set, &nt);
	markset_add(set, &nb);
	markset_add(set, &nm);
}

void markset_clear(markset *set)
{
	dnode *dn;
	while((dn = dlist_first(set->dl)) != NULL) {
		free(dnode_data(dn).v);
		dnode_rem(dn);
	}
}

int markset_iter(markset *set, iter *i)
{
	return dlist_iter(set->dl, i);
}
