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
int del = 0;
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

void drawrect(rect *r, color cc)
{
	int i, ix, iy;
	color c = cc | 0x88000000;

	for(i = 0; i < canvas->w * canvas->h; i++)
		canvas->pix[i] = 0;

	for(ix = r->x; ix < r->x + r->w; ix++)
	for(iy = r->y; iy < r->y + r->h; iy++)
		dot(ix, iy, cc | 0x44000000);

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
		drawrect(r, 0x0000FF);
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
	if(del)
		drawrect(&r, 0xFF0000);
	else
		drawrect(&r, 0x0000FF);
}

void eventloop(void)
{
	event ev;
	mouseevent mev;
	rect r;
	int update;

	ox = oy = mx = my = drag = 0;

	while(event_wait(&ev)) {
		switch(ev.type) {
		case EVENT_MOUSE:
			mev = ev.mouse;
			update = 0;
			mx = mev.x;
			my = mev.y;
			if(!drag && mev.state) {
				ox = mx;
				oy = my;
				drag = 1;
			}
			if(drag)
				update = 1;
			if(drag && !mev.state) {
				drag = 0;
				r = ps2rect(ox, oy, mx, my);
				if(!del)
					markset_add(set, &r);
				else
					markset_cut(set, &r);
			}
			break;
		case EVENT_KEY:
			if(ev.key.sym == 'd')
				del = 1;
			if(ev.key.sym == 'f')
				del = 0;
			break;
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
