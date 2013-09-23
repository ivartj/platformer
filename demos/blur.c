#include <video.h>
#include <log.h>
#include <stdio.h>
#include <stdlib.h>
#include <markset.h>
#include <event.h>
#include <gfx.h>
#include <rect.h>

markset *marks = NULL;
rect view = { 0 };

void draw(gfx *g, rect *r)
{

}

void redraw(void)
{
	gfx *g;
	iter i;
	rect *m;

	g = gfx_create(video);
	markset_iter(marks, &i);
	while(iterate(&i, (data *)(void **)&m))
		draw(g, m);
	markset_clear(marks);
	gfx_destroy(g);
	video_update();
}

void eventloop(void)
{
	event ev;

	while(event_wait(&ev))
	switch(ev.type) {
	case EVENT_MOUSE:
		redraw();
		break;
	}
}

int main(int argc, char *argv[])
{
	int err;

	err = video_init(640, 480);
	if(err)
		error("Failed to initialize video: %s", geterrmsg());

	view.w = video->w;
	view.h = video->h;

	marks = markset_create();
	markset_add(marks, &view);

	eventloop();

	exit(EXIT_SUCCESS);
}
