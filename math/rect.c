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
