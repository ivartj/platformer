#include <video.h>
#include <color.h>
#include <log.h>
#include <event.h>
#include <thread.h>
#include <markset.h>
#include <mapset.h>
#include <gfx.h>

typedef struct _graphic graphic;
typedef struct _circle circle;
typedef void (*drawfn)(gfx *x, rect *r, void *data);

struct _graphic {
	void *data;
	drawfn draw;
};

struct _circle {
	int x, y;
	double r;
	color c;
};

mapset *maps = NULL;
markset *marks = NULL;

void draw(gfx *x, rect *r)
{

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

	for(;;) {
		while(event_poll(&ev)) {

		}
		redraw();
		thread_sleep(30);
	}
}

int main(int argc, char *argv[])
{
	int err;

	err = video_init(640, 480);
	if(err)
		error("video_init: %s", geterrmsg());

	maps = mapset_create();
	marks = markset_create();

	eventloop();
	return 0;
}
