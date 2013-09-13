#ifndef RECT_H
#define RECT_H

typedef struct {
	int x, y, w, h;
} rect;

rect ps2rect(int x, int y, int x2, int y2);
int rect_isoverlap(rect *a, rect *b);
int rect_iswithin(rect *a, rect *b);

#endif
