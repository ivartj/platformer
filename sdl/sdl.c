#include "video.h"
#include "log.h"
#include "event.h"
#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <rect.h>
#include <color.h>

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
	video->private = canvas;
}

void unlockcanvas(void)
{
	if(SDL_MUSTLOCK(canvas))
		SDL_UnlockSurface(canvas);
}

/* update display with data in video image */
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
	SDL_Surface *surf;

	img = calloc(1, sizeof(image));
	img->private = surf = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);
	img->w = w;
	img->h = h;
	if(SDL_MUSTLOCK(surf))
		SDL_LockSurface(surf);
	img->pix = surf->pixels;

	return img;
}

image *mask_create(int w, int h)
{
	image *img;
	SDL_Surface *sdlsurf;

	img = image_create(w, h);
	sdlsurf = (SDL_Surface *)(img->private);
	SDL_SetAlpha(sdlsurf, SDL_RLEACCEL | SDL_SRCALPHA, 255);

	return img;
}

void image_destroy(image *img)
{
	SDL_Surface *surf;

	surf = (SDL_Surface *)(img->private);
	if(SDL_MUSTLOCK(surf))
		SDL_UnlockSurface(surf);
	SDL_FreeSurface(surf);
	free(img);
}

void image_fill(image *img, rect *r, color c)
{
	SDL_Surface *surf;
	SDL_Rect sdlr;

	sdlr.x = r->x;
	sdlr.y = r->y;
	sdlr.w = r->w;
	sdlr.h = r->h;

	surf = (SDL_Surface *)(img->private);
	if(SDL_MUSTLOCK(surf))
		SDL_UnlockSurface(surf);

	SDL_FillRect(surf, &sdlr, c);

	if(SDL_MUSTLOCK(surf))
		SDL_LockSurface(surf);
}

int blit(image *src, image *dest)
{
	SDL_Surface *sdlsrc, *sdldest;
	int err;

	sdlsrc = (SDL_Surface *)(src->private);
	sdldest = (SDL_Surface *)(dest->private);
	if(SDL_MUSTLOCK(sdlsrc))
		SDL_UnlockSurface(sdlsrc);
	if(SDL_MUSTLOCK(sdldest))
		SDL_UnlockSurface(sdldest);

	err = SDL_BlitSurface((SDL_Surface *)(src->private), NULL, (SDL_Surface *)(dest->private), NULL);
	if(err)
		err = seterrmsg("SDL_BlitSurface: %s", SDL_GetError());

	if(SDL_MUSTLOCK(sdlsrc))
		SDL_LockSurface(sdlsrc);
	if(SDL_MUSTLOCK(sdldest))
		SDL_LockSurface(sdldest);

	return err;
}

int blitxy(image *src, image *dest, int x, int y)
{
	SDL_Rect rect = { 0 };
	SDL_Surface *sdlsrc, *sdldest;
	int err;

	rect.x = x;
	rect.y = y;

	sdlsrc = (SDL_Surface *)(src->private);
	sdldest = (SDL_Surface *)(dest->private);
	if(SDL_MUSTLOCK(sdlsrc))
		SDL_UnlockSurface(sdlsrc);
	if(SDL_MUSTLOCK(sdldest))
		SDL_UnlockSurface(sdldest);

	err = SDL_BlitSurface((SDL_Surface *)(src->private), NULL, (SDL_Surface *)(dest->private), &rect);
	if(err)
		err = seterrmsg("SDL_BlitSurface: %s", SDL_GetError());

	if(SDL_MUSTLOCK(sdlsrc))
		SDL_LockSurface(sdlsrc);
	if(SDL_MUSTLOCK(sdldest))
		SDL_LockSurface(sdldest);

	return err;
}

int blitrxy(image *src, rect *r, image *dest, int x, int y)
{
	SDL_Rect sdlr = { 0 };
	SDL_Rect sdlxy = { 0 };
	SDL_Surface *sdlsrc, *sdldest;
	int err;

	sdlr.x = r->x;
	sdlr.y = r->y;
	sdlr.w = r->w;
	sdlr.h = r->h;
	sdlxy.x = x;
	sdlxy.y = y;

	sdlsrc = (SDL_Surface *)(src->private);
	sdldest = (SDL_Surface *)(dest->private);
	if(SDL_MUSTLOCK(sdlsrc))
		SDL_UnlockSurface(sdlsrc);
	if(SDL_MUSTLOCK(sdldest))
		SDL_UnlockSurface(sdldest);

	err = SDL_BlitSurface((SDL_Surface *)(src->private), &sdlr, (SDL_Surface *)(dest->private), &sdlxy);
	if(err)
		err = seterrmsg("SDL_BlitSurface: %s", SDL_GetError());

	if(SDL_MUSTLOCK(sdlsrc))
		SDL_LockSurface(sdlsrc);
	if(SDL_MUSTLOCK(sdldest))
		SDL_LockSurface(sdldest);

	return err;
}

int blitxymask(image *src, image *dest, int x, int y, image *mask)
{
	// TODO
	return seterrmsg("Unimplemented method blitxymask");
}

static int event_translate(event *ev, SDL_Event *sdlev)
{
	switch(sdlev->type) {
	case SDL_KEYDOWN:
		{
			keyevent *kbev;
			kbev = &(ev->key);
			kbev->type = EVENT_KEY;
			kbev->code = sdlev->key.keysym.scancode;
			kbev->mod = sdlev->key.keysym.mod;
			kbev->sym = sdlev->key.keysym.sym;
		}
		return 0;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		{
			mouseevent *mouseev;
			mouseev = &(ev->mouse);
			mouseev->type = EVENT_MOUSE;
			if(sdlev->type == SDL_MOUSEBUTTONDOWN) {
				mouseev->state = sdlev->button.state;
				mouseev->x = sdlev->button.x;
				mouseev->y = sdlev->button.y;
			} else {
				mouseev->state = sdlev->motion.state;
				mouseev->x = sdlev->motion.x;
				mouseev->y = sdlev->motion.y;
			}
		}
		return 0;
	case SDL_QUIT:
		exit(EXIT_SUCCESS);
	default:
		return 1;
	}
}

int event_wait(event *ev)
{
	SDL_Event sdlev;
	int ok;
	int cont;

	do {
		cont = 0;

		ok = SDL_WaitEvent(&sdlev);
		if(!ok)
			return 0;

		cont = event_translate(ev, &sdlev);
	} while(cont);

	return 1;
}

int event_poll(event *ev)
{
	SDL_Event sdlev;
	int ok;
	int cont;

	do {
		cont = 0;

		ok = SDL_PollEvent(&sdlev);
		if(!ok)
			return 0;

		cont = event_translate(ev, &sdlev);
	} while(cont);

	return 1;
}
