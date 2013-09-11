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
image *mask_create(int w, int h);

int blit(image *src, image *dest);
int blitxy(image *src, image *dest, int x, int y);
int blitxymask(image *src, image *dest, int x, int y, image *mask);

#endif
