#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct _image image;

struct _image {
	int w, h;
	uint32_t *pix;
	void *private;
};

image *image_create(int w, int h);

void blit(image *src, image *dest);
void blitxy(image *src, image *dest, int x, int y);
void blitxymask(image *src, image *dest, int x, int y, image *mask);

#endif
