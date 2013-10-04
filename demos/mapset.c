#include <load.h>
#include <video.h>
#include <string.h>
#include <mapset.h>
#include <markset.h>
#include <gfx.h>
#include <log.h>
#include <color.h>
#include <libgen.h>
#include <event.h>
#include <sys/param.h>
#include <math.h>

char *dir;

typedef struct _object object;
struct _object {
	data d;
	void (*draw)(object *o, gfx *g, rect *r);
	void (*move)(object *o, int x, int y);
	map *m;
};

markset *marks = NULL;
mapset *maps = NULL;
rect view;
object *xor;

color xor_color(color c)
{
	return 0x00FFFFFF ^ c;
}

color purered_color(color c)
{
	return c & 0xFFFF0000;
}

color rightshift_color(color c)
{
	c <<= 8;
	c |= (c & 0xFF000000) >> 24;
	c |= 0xFF000000;
	return c;
}

color black_color(color c)
{
	return 0xFF000000;
}

color redgreen_color(color c)
{
	double r, g, b;

	getrgb(c, &r, &g, &b);

	g = b = (g + b) / 2.0;

	return rgb(r, g, b);
}

void rect_draw(object *o, gfx *g, rect *r)
{
	int x, y;
	color (*color_filter)(color c);

	color_filter = o->d.v;

	for(x = r->x; x < r->x + r->w; x++)
	for(y = r->y; y < r->y + r->h; y++) {
		gfx_color(g, color_filter(gfx_get(g, x, y)));
		gfx_dot(g, x, y);
	}
}

void filter_rect_move(object *o, int x, int y)
{
	rect r;
	rect top, bot, rig, lef;

	r.x = x;
	r.y = y;
	rect_xor(map_rect(o->m), &r, &top, &lef, &rig, &bot);
	markset_add(marks, top);
	markset_add(marks, lef);
	markset_add(marks, rig);
	markset_add(marks, bot);
	map_setrect(o->m, &r);
}

void sprite_draw(object *o, gfx *g, rect *r)
{
	image *img;
	rect s;

	img = o->d.v;
	memcpy(&s, r, sizeof(s));
	s.x -= map_rect(o->m)->x;
	s.y -= map_rect(o->m)->y;
	gfx_blit(g, img, &s, r->x, r->y);
}

typedef struct _circ_data circ_data;
struct _circ_data {
	color (*color_filter)(color c);
	int x, y;
	rect top;
	rect lef;
	rect rig;
	rect cen;
	rect bot;
	map *mtop;
	map *mlef;
	map *mrig;
	map *mcen;
	map *mbot;
	double r;
};

color alpha_mix(color d, color c, double a)
{
	double dr, dg, db, cr, cg, cb;
	dr = (d & 0x00FF0000) >> 16;
	dg = (d & 0x0000FF00) >>  8;
	db = (d & 0x000000FF) >>  0;
	dr /= 255;
	dg /= 255;
	db /= 255;
	cr = (c & 0x00FF0000) >> 16;
	cg = (c & 0x0000FF00) >>  8;
	cb = (c & 0x000000FF) >>  0;
	cr /= 255;
	cg /= 255;
	cb /= 255;
	
	return rgb(
		dr * a + (1 - a) * cr,
		dg * a + (1 - a) * cg,
		db * a + (1 - a) * cb);
}

void circ_draw(object *o, gfx *g, rect *r)
{
	int x, y;
	circ_data *d;
	double l;
	d = (circ_data *)o->d.v;

	for(y = r->y; y < r->y + r->h; y++) {
		
		for(x = r->x; x < r->x + r->w; x++) {
			l = sqrt(pow(x - d->x, 2) + pow(y - d->y, 2));
			if((l = d->r - l) < 0)
				continue;
			l = fmin(1.0, l);
			gfx_color(g, alpha_mix(d->color_filter(gfx_get(g, x, y)), gfx_get(g, x, y), l));
			gfx_dot(g, x, y);
		}
	}
}

void circ_move(object *o, int x, int y)
{
	circ_data *d;
	int dx, dy;
	rect cen;
	rect ctop, cbot, crig, clef;


	d = (circ_data *)(o->d.v);
	markset_add(marks, d->top);
	markset_add(marks, d->bot);
	markset_add(marks, d->lef);
	markset_add(marks, d->rig);

	memcpy(&cen, &(d->cen), sizeof(cen));

	dx = x - d->x;
	dy = y - d->y;

	d->top.x += dx;
	d->bot.x += dx;
	d->lef.x += dx;
	d->rig.x += dx;
	d->cen.x += dx;
	d->top.y += dy;
	d->bot.y += dy;
	d->lef.y += dy;
	d->rig.y += dy;
	d->cen.y += dy;

	map_setrect(d->mtop, &(d->top));
	map_setrect(d->mbot, &(d->bot));
	map_setrect(d->mcen, &(d->cen));
	map_setrect(d->mlef, &(d->lef));
	map_setrect(d->mrig, &(d->rig));

	d->x = x;
	d->y = y;

	markset_add(marks, d->top);
	markset_add(marks, d->bot);
	markset_add(marks, d->lef);
	markset_add(marks, d->rig);

	rect_xor(&cen, &(d->cen), &ctop, &clef, &crig, &cbot);
	markset_add(marks, ctop);
	markset_add(marks, clef);
	markset_add(marks, crig);
	markset_add(marks, cbot);
}

object *filter_circ(int x, int y, double r, color (*draw)(color c))
{
	object *o;
	circ_data *d;
	int rat;

	o = calloc(1, sizeof(object));
	o->draw = circ_draw;
	o->move = circ_move;
	d = calloc(1, sizeof(circ_data));
	rat = r / sqrt(2);

	d->r = r;
	d->x = x;
	d->y = y;
	d->color_filter = draw;

	d->top.y = y - r;
	d->top.x = x - rat;
	d->top.w = rat * 2;
	d->top.h = r - rat;

	d->cen.y = y - rat;
	d->cen.x = x - rat;
	d->cen.w = rat * 2;
	d->cen.h = rat * 2;

	d->lef.x = x - r;
	d->lef.y = y - rat;
	d->lef.w = r - rat;
	d->lef.h = rat * 2;

	d->rig.x = x + rat;
	d->rig.y = y - rat;
	d->rig.w = r - rat;
	d->rig.h = rat * 2;

	d->bot.x = x - rat;
	d->bot.y = y + rat;
	d->bot.w = rat * 2;
	d->bot.h = r - rat;

	/* margin */
	d->top.x -= 1;
	d->top.w += 2;
	d->bot.x -= 1;
	d->bot.w += 2;
	d->cen.x += 1;
	d->cen.w -= 2;
	d->lef.w += 1;
	d->rig.x -= 1;
	d->rig.w += 1;

	d->mtop = mapset_add(maps, &(d->top), o);
	d->mbot = mapset_add(maps, &(d->bot), o);
	d->mcen = mapset_add(maps, &(d->cen), o);
	d->mlef = mapset_add(maps, &(d->lef), o);
	d->mrig = mapset_add(maps, &(d->rig), o);
	markset_add(marks, d->top);
	markset_add(marks, d->bot);
	markset_add(marks, d->cen);
	markset_add(marks, d->lef);
	markset_add(marks, d->rig);

	o->d = (data)(void *)d;

	return o;
}

void sprite_move(object *o, int x, int y)
{
	
}

object *sprite(int x, int y, const char *path)
{
	image *img;
	object *o;
	rect r;
	char abspath[MAXPATHLEN];

	sprintf(abspath, "%s/%s", dir, path);

	img = load_image(abspath);
	if(img == NULL)
		error("Failed to load PNG image: %s", geterrmsg());
	r.x = x;
	r.y = y;
	r.w = img->w;
	r.h = img->h;
	o = calloc(1, sizeof(object));
	o->d = (data)(void *)img;
	o->draw = sprite_draw;
	o->move = sprite_move;
	o->m = mapset_add(maps, &r, o);
	return o;
}

object *filter_rect(int x, int y, int w, int h, color (*draw)(color c))
{
	rect r;
	object *o;

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	o = calloc(1, sizeof(object));
	o->draw = rect_draw;
	o->d.v = (void *)draw;
	o->move = filter_rect_move;
	o->m = mapset_add(maps, &r, o);
	return o;
}

void bg(gfx *g, rect *r)
{
	int x, y;
	double cx, cy;
	double len;
	double mlen;

	cx = video->w / 2;
	cy = video->h / 2;
	mlen = (video->w < video->h ? video->w : video->h) / 2;

	for(x = r->x; x < r->x + r->w; x++)
	for(y = r->y; y < r->y + r->h; y++) {
		len = sqrt(pow(x - cx, 2) + pow(y - cy, 2)) / mlen;
		len = exp(-pow(len, 2) / (2 * pow(1.0 / 3.0, 2)));
		len = fmin(1.0, len);
		len = 1 - len;
		gfx_color(g, rgb(len, len, len));
		gfx_dot(g, x, y);
	}
}

void draw(gfx *x, rect *r)
{
	rect o;
	map *m;
	iter i;
	object *obj;

	bg(x, r);

	mapset_iter_overlap(maps, &i, r);
	while(iterate(&i, (data *)(void **)&m)) {
		rect_overlap(map_rect(m), r, &o);
		map_data(m, &obj);
		obj->draw(obj, x, &o);
	}
}

void redraw(void)
{
	gfx *x;
	rect r;
	data d;
	iter i;

	x = gfx_create(video);

	markset_iter(marks, &i);
	while(iterate(&i, &r)) {
		draw(x, &r);
	}
	markset_clear(marks);

	gfx_destroy(x);
	video_update();
}

void eventloop(void)
{
	event ev;
	int update;

	while(event_wait(&ev)) {
		update = 0;

		do {
			switch(ev.type) {
			case EVENT_KEY:
				break;
			case EVENT_MOUSE:
				circ_move(xor, ev.mouse.x, ev.mouse.y);
				update = 1;
				break;
			}
		} while(event_poll(&ev));

		if(update)
			redraw();
	}
}

int main(int argc, char *argv[])
{
	int err;

	if(argc >= 1)
		dir = dirname(argv[0]);
	else
		dir = "";

	err = video_init(640, 480);
	if(err)
		error("Failed to initialize video: %s", geterrmsg());

	marks = markset_create();
	maps = mapset_create();

	view.x = view.y = 0;
	view.w = video->w;
	view.h = video->h;

	markset_add(marks, view);
	xor = filter_circ(200, 200, 150, xor_color);
	filter_rect(0, 0, 320, 480, xor_color);

	sprite(0, 0, "chooyu.png");

	redraw();
	eventloop();

	return 0;
}
