#include "color.h"
#include <stdio.h>

color rgba(double r, double g, double b, double a)
{
	color c;

	c = 0;
	c |= (uint8_t)(a * 255.0); c <<= 8;
	c |= (uint8_t)(r * 255.0); c <<= 8;
	c |= (uint8_t)(g * 255.0); c <<= 8;
	c |= (uint8_t)(b * 255.0);

	return c;
}

color rgb(double r, double g, double b)
{
	color c;
	double a;

	a = 1.0;

	c = 0;
	c |= (uint8_t)(a * 255.0); c <<= 8;
	c |= (uint8_t)(r * 255.0); c <<= 8;
	c |= (uint8_t)(g * 255.0); c <<= 8;
	c |= (uint8_t)(b * 255.0);

	return c;
}

void getrgb(color c, double *r, double *g, double *b)
{
	*r = (double)((0x00FF0000 & c) >> 16) / 255.0;
	*g = (double)((0x0000FF00 & c) >>  8) / 255.0;
	*b = (double)((0x000000FF & c) >>  0) / 255.0;
}
