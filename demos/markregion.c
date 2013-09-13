/* This demo illustrates a method that could be used to mark rectangular regions
 * in a drawing buffer as needing redrawing. As it takes in rectangular regions
 * it splits and rearranges them as needed so that none of the resulting
 * regions overlap. This could help optimize graphics as no area is redrawn
 * twice during a cycle, and only the areas within the regions are redrawn.
 */


#include <video.h>
#include <stdlib.h>
#include <log.h>
#include <color.h>
#include <event.h>
#include <dlist.h>

typedef struct _rect rect;

struct _rect {
	int x, y, w, h;
};

int ox, oy;
int mx, my;
int drag;
image *canvas;
dlist *marks = NULL;

static void mark(rect *r);
static void add(rect *r);

int overlap(rect *m, rect *r)
{
	if(r->x >= m->x + m->w)
		return 0;
	if(m->x >= r->x + r->w)
		return 0;
	if(r->y >= m->y + m->h)
		return 0;
	if(m->y >= r->y + r->h)
		return 0;
	return 1;
}

int within(rect *m, rect *r)
{
	if(m->x > r->x)
		return 0;
	if(m->x + m->w < r->x + r->w)
		return 0;
	if(m->y > r->y)
		return 0;
	if(m->y + m->h < r->y + r->h)
		return 0;
	return 1;
}

void split(rect *a, rect *c)
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
	mark(&nt);
	mark(&nb);
	mark(&nm);
}

void mark(rect *r)
{
	rect *m;
	dnode *dn;

	if(r->w == 0 || r->h == 0)
		return;

	for(dn = dlist_first(marks); dn != NULL; dn = dnode_next(dn)) {
		m = (rect *)dnode_data(dn);
		if(within(m, r))
			return;
		if(overlap(m, r)) {
			dnode_rem(dn);
			split(m, r);
			free(m);
			return;
		}
	}

	add(r);
}

void add(rect *r)
{
	rect *m;
	m = calloc(1, sizeof(rect));
	*m = *r;
	dlist_add_first(marks, m);
}

rect p2rect(int x, int y, int x2, int y2)
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

void dot(int x, int y, color c)
{
	if(x < 0 || y < 0)
		return;
	if(x >= video->w || y >= video->h)
		return;
	canvas->pix[x + y * video->w] = c;
}

void drawrect(rect *r)
{
	int i, ix, iy;
	color c = 0x440000FF;

	for(i = 0; i < canvas->w * canvas->h; i++)
		canvas->pix[i] = 0;

	for(ix = r->x; ix < r->x + r->w; ix++)
	for(iy = r->y; iy < r->y + r->h; iy++)
		dot(ix, iy, 0x220000FF);

	for(i = r->x; i < r->x + r->w; i++) {
		dot(i, r->y, c);
		dot(i, r->y + r->h, c);
	}

	for(i = r->y; i < r->y + r->h; i++) {
		dot(r->x, i, c);
		dot(r->x + r->w, i, c);
	}

	blit(canvas, video);
}

void drawmarks(void)
{
	rect *r;
	dnode *i;

	for(i = dlist_first(marks); i != NULL; i = dnode_next(i)) {
		r = (rect *)dnode_data(i);
		drawrect(r);
	}
}

void redraw(void)
{
	rect r;
	int x, y;

	for(x = 0; x < video->w; x++)
	for(y = 0; y < video->h; y++)
		video->pix[x + y * video->w] = 0xFFFFFFFF;

	drawmarks();
	if(!drag)
		return;

	r = p2rect(ox, oy, mx, my);
	drawrect(&r);
}

void eventloop(void)
{
	mouseevent ev;
	rect r;

	ox = oy = mx = my = drag = 0;

	while(event_wait((event *)&ev))
	if(ev.type == EVENT_MOUSE) {
		mx = ev.x;
		my = ev.y;
		if(!drag && ev.state) {
			ox = mx;
			oy = my;
			drag = 1;
		}
		if(drag && !ev.state) {
			drag = 0;
			r = p2rect(ox, oy, mx, my);
			mark(&r);
		}
		redraw();
		video_update();
	}
}

int main(int argc, char *argv[])
{
	int err;

	err = video_init(640, 480);
	if(err)
		error("Failed to initialize video: %s", geterrmsg());
	canvas = image_create(video->w, video->h);

	marks = dlist_create();

	eventloop();
	exit(EXIT_SUCCESS);
}
