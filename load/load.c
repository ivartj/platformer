#include <FreeImage.h>
#include <stdlib.h>
#include <log.h>
#include <image.h>
#include <color.h>
#include <string.h>

static void seterrmsg_freeimage(FREE_IMAGE_FORMAT fif, const char *message)
{
	seterrmsg("%s", message);
}

int load_init(void)
{
	FreeImage_Initialise(FALSE);
	atexit(FreeImage_DeInitialise);
	FreeImage_SetOutputMessage(seterrmsg_freeimage);
	return 0;
}

image *load_image(const char *path)
{
	FIBITMAP *src;
	FIBITMAP *dest;
	int width, height;
	int ok;
	int x, y;
	image *val = NULL;
	color *bitmap;

	src = FreeImage_Load(FIF_PNG, path, 0);
	if(src == NULL) {
		seterrmsg("FreeImage_Load: %s", geterrmsg());
		return NULL;
	}

	width = FreeImage_GetWidth(src);
	height = FreeImage_GetHeight(src);
	dest = FreeImage_Allocate(width, height, 32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF);

	ok = FreeImage_Paste(dest, src, 0, 0, 255);
	if(!ok) {
		seterrmsg("FreeImage_Paste: %s", geterrmsg());
		FreeImage_Unload(src);
		FreeImage_Unload(dest);
		return NULL;
	}

	FreeImage_Unload(src);

	bitmap = (color *)FreeImage_GetBits(dest);
	val = image_create(width, height);

	for(y = 0; y < height; y++)
		memcpy(&(val->pix[y * width]), &(bitmap[(height - 1 - y) * width]), width * sizeof(color));

	FreeImage_Unload(dest);

	return val;
}
