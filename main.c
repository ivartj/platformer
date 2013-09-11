#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <math.h>

#include "log.h"
#include "video.h"
#include "event.h"
#include "color.h"

void usage(FILE *out)
{
	fprintf(out, "usage: game\n");
}

void parseargs(int argc, char *argv[])
{
	int c;
	static struct option longopts[] = {
		{ "help", no_argument, NULL, 'h' },
		{ 0, 0, 0, 0 },
	};

	while((c = getopt_long(argc, argv, "h", longopts, NULL)) != -1)
	switch(c) {
	case 'h':
		usage(stdout);
		exit(EXIT_SUCCESS);
	case '?':
	case ':':
		usage(stderr);
		exit(EXIT_FAILURE);
	}

	switch(argc - optind) { // how many arguments are left
	case 0:
		break;
	default:
		usage(stderr);
		exit(EXIT_FAILURE);
	}
}

void init(void)
{
	int err;

	err = video_init(640, 480);
	if(err)
		error("Failed to initialize video: %s", geterrmsg());
}

void fail(void)
{
	exit(EXIT_FAILURE);
}

void quit(void)
{
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	parseargs(argc, argv);
	init();

	int x, y;
	image *img = image_create(100, 100);
	int i, j = 0;
	for(x = 0; x < img->w; x++)
	for(y = 0; y < img->h; y++)
		img->pix[x + y * img->w] = (0x11111100 * (y % 16));
	double r;
	image *mask = image_create(100, 100);
	for(x = 0; x < mask->w; x++)
	for(y = 0; y < mask->h; y++) {
		r = fmin(1.0,
			sqrt(pow(x - mask->w / 2, 2) + pow(y - mask->h / 2, 2))
			/ (fmin(mask->w, mask->h) / 2));
		r = exp(-pow(r, 2) / pow(1.0 / 3.0, 2));
		mask->pix[x + y * mask->w] = rgba(1, 1, 1, r);
	}

	event ev;
	int err;
	while(event_wait(&ev)) {
		for(i = 0; i < video->w * video->h; i++)
			video->pix[i] = 0xFF000000 | (0x00000011 * (i % 16));
		err = blit(img, mask);
		if(err)
			warning("Blit failed: %s", geterrmsg());
		err = blitxy(mask, video, j++, j++);
		if(err)
			warning("Blit failed: %s", geterrmsg());
		video_update();
	}

	quit();
	return 0;
}
