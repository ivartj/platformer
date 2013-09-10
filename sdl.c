#include "video.h"
#include "log.h"
#include <SDL.h>
#include <stdlib.h>

static void lockcanvas(void);
static void unlockcanvas(void);

image *video = NULL;
static int sdl_initialized = 0;
static SDL_Surface *canvas = NULL;
static SDL_Surface *window = NULL;

int sdl_init(void)
{
	int err;

	if(sdl_initialized)
		return 0;

	err = SDL_Init(SDL_INIT_VIDEO);
	if(err)
		return seterrmsg("SDL failed to initialize: %s", SDL_GetError());
	atexit(SDL_Quit);
	sdl_initialized = 1;

	return 0;
}

/* returns -1 on failure */
int video_init(int w, int h)
{
	int err;

	err = sdl_init();
	if(err)
		return err;

	canvas = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);
	if(canvas == NULL)
		return seterrmsg("Failed to create SDL canvas: %s", SDL_GetError()); 

	window = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE);
	if(window == NULL)
		return seterrmsg("Failed to open SDL window: %s", SDL_GetError());

	lockcanvas();

	return 0;
}

void lockcanvas(void)
{
	if(SDL_MUSTLOCK(canvas))
		SDL_LockSurface(canvas);

	if(video == NULL)
		video = calloc(1, sizeof(image));

	video->pix = canvas->pixels;
	video->w = canvas->w;
	video->h = canvas->h;
}

void unlockcanvas(void)
{
	if(SDL_MUSTLOCK(canvas))
		SDL_UnlockSurface(canvas);
}

/* update display with date in video image */
void video_update(void)
{
	unlockcanvas();
	SDL_BlitSurface(canvas, NULL, window, NULL);
	SDL_Flip(window);
	lockcanvas();
}

image *image_create(int w, int h)
{
	image *img;

	img = calloc(1, sizeof(image));
	img->private = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);
	img->w = w;
	img->h = h;
	img->pix = ((SDL_Surface *)(img->private))->pixels;

	return img;
}

void blit(image *src, image *dest)
{
	SDL_BlitSurface((SDL_Surface *)(src->private), NULL, (SDL_Surface *)(dest->private), NULL);
}

void blitxy(image *src, image *dest, int x, int y)
{
	SDL_Rect rect = { 0 };

	rect.x = x;
	rect.y = y;

	SDL_BlitSurface((SDL_Surface *)(src->private), NULL, (SDL_Surface *)(dest->private), &rect);
}

void blitxymask(image *src, image *dest, int x, int y, image *mask)
{
	// TODO
}
