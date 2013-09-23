#include "gfx.h"
#include <string.h>
#include <image.h>

struct _gfx {
	image *img;
	int x, y;
	color c;
};

gfx *gfx_create(image *img)
{
	gfx *g;

	g = calloc(1, sizeof(gfx));
	g->img = img;

	return g;
}

void gfx_destroy(gfx *g)
{
	free(g);
}

void gfx_translate(gfx *g, int x, int y)
{
	g->x += x;
	g->y += y;
}

void gfx_color(gfx *g, color c)
{
	g->c = c;
}

void gfx_dot(gfx *g, int x, int y)
{
	int tx, ty;

	tx = x + g->x;
	ty = y + g->y;
	if(tx < 0 || tx >= g->img->w)
		return;
	if(ty < 0 || ty >= g->img->h)
		return;
	g->img->pix[tx + ty * g->img->w] = g->c;
}

void gfx_fill(gfx *g, rect *r)
{
	rect rr;

	memcpy(&rr, r, sizeof(rr));
	rr.x += g->x;
	rr.y += g->y;
	image_fill(g->img, &rr, g->c);
}

color gfx_get(gfx *g, int x, int y)
{
	x += g->x;
	y += g->y;
	if(x < 0 || x >= g->img->w)
		return 0;
	if(y < 0 || y >= g->img->h)
		return 0;
	return g->img->pix[x + g->img->w * y];
}

void gfx_blit(gfx *g, image *img, rect *r, int x, int y)
{
	blitrxy(img, r, g->img, g->x + x, g->y + y);
}
