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
#include <markset.h>

int ox, oy;
int mx, my;
int drag;
image *canvas;
markset *set;

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
	iter i;
	int n = 0;

	markset_iter(set, &i);
	while(iterate(&i, (data *)(void **)(&r))) {
		n++;
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

	r = ps2rect(ox, oy, mx, my);
	drawrect(&r);
}

void eventloop(void)
{
	mouseevent ev;
	rect r;
	int update;

	ox = oy = mx = my = drag = 0;

	while(event_wait((event *)&ev))
	if(ev.type == EVENT_MOUSE) {
		update = 0;
		mx = ev.x;
		my = ev.y;
		if(!drag && ev.state) {
			ox = mx;
			oy = my;
			drag = 1;
		}
		if(drag)
			update = 1;
		if(drag && !ev.state) {
			drag = 0;
			r = ps2rect(ox, oy, mx, my);
			markset_add(set, &r);
		}
		if(update) {
			redraw();
			video_update();
		}
	}
}

int main(int argc, char *argv[])
{
	int err;

	err = video_init(640, 480);
	if(err)
		error("Failed to initialize video: %s", geterrmsg());
	canvas = image_create(video->w, video->h);

	set = markset_create();

	eventloop();
	exit(EXIT_SUCCESS);
}
