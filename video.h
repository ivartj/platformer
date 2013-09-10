#ifndef VIDEO_H
#define VIDEO_H

#include "image.h"

extern image *video;

/* returns -1 on failure */
int video_init(int w, int h);

/* update display with date in video image */
void video_update(void);

#endif
