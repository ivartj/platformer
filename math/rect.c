#include "rect.h"

rect ps2rect(int x, int y, int x2, int y2)
{
	rect r;

	if(x < x2) {
		r.x = x;
		r.w = x2 - x;
	} else {
		r.x = x2;
		r.w = x - x2;
	}

	if(y < y2) {
		r.y = y;
		r.h = y2 - y;
	} else {
		r.y = y2;
		r.h = y - y2;
	}

	return r;
}

int rect_isoverlap(rect *a, rect *b)
{
	if(b->x >= a->x + a->w)
		return 0;
	if(a->x >= b->x + b->w)
		return 0;
	if(b->y >= a->y + a->h)
		return 0;
	if(a->y >= b->y + b->h)
		return 0;
	return 1;
}

int rect_iswithin(rect *a, rect *b)
{
	if(a->x > b->x)
		return 0;
	if(a->x + a->w < b->x + b->w)
		return 0;
	if(a->y > b->y)
		return 0;
	if(a->y + a->h < b->y + b->h)
		return 0;
	return 1;
}

void rect_overlap(rect *a, rect *b, rect *o)
{
	o->x = a->x > b->x ? a->x : b->x;
	o->y = a->y > b->y ? a->y : b->y;
	o->w = (a->x + a->w < b->x + b->w ? a->x + a->w : b->x + b->w) - o->x;
	o->h = (a->y + a->h < b->y + b->h ? a->y + a->h : b->y + b->h) - o->y;
}

void rect_xor(rect *a, rect *c, rect *t, rect *l, rect *r, rect *b)
{
	rect *tmp;

	if(c->y < a->y) {
		tmp = a;
		a = c;
		c = tmp;
	}
	
	t->y = a->y;
	t->h = c->y - a->y;
	t->x = a->x;
	t->w = a->w;

	if(c->y + c->h < a->y + a->h) {
		tmp = a;
		a = c;
		c = tmp;
	}

	b->y = a->y + a->h;
	b->h = c->y + c->h - b->y;
	b->x = c->x;
	b->w = c->w;

	if(c->x < a->x) {
		tmp = a;
		a = c;
		c = tmp;
	}

	l->x = a->x;
	l->w = c->x - a->x;
	l->y = t->y + t->h;
	l->h = b->y - l->y;

	if(c->x + c->w < a->x + a->w) {
		tmp = a;
		a = c;
		c = tmp;
	}

	r->x = a->x + a->w;
	r->w = c->x + c->w - r->x;
	r->y = l->y;
	r->h = l->h;
}
