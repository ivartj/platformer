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
	quit();
	return 0;
}
