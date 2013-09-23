#ifndef GFX_H
#define GFX_H

#include <image.h>
#include <color.h>
#include <rect.h>

typedef struct _gfx gfx;

gfx *gfx_create(image *img);
void gfx_destroy(gfx *g);

void gfx_translate(gfx *g, int x, int y);
void gfx_color(gfx *g, color c);

void gfx_dot(gfx *g, int x, int y);
color gfx_get(gfx *g, int x, int y);
void gfx_fill(gfx *g, rect *r);
void gfx_blit(gfx *g, image *img, rect *r, int x, int y);

#endif
