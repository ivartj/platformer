#ifndef RECT_H
#define RECT_H

typedef struct {
	int x, y, w, h;
} rect;

rect ps2rect(int x, int y, int x2, int y2);

int rect_isoverlap(rect *a, rect *b);
void rect_overlap(rect *a, rect *b, rect *o);
int rect_iswithin(rect *a, rect *b);
void rect_xor(rect *a, rect *c, rect *t, rect *l, rect *r, rect *b);

#endif
