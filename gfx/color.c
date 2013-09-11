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
	c |= (uint8_t)(a * 255.0); c <<= 4;
	c |= (uint8_t)(r * 255.0); c <<= 4;
	c |= (uint8_t)(g * 255.0); c <<= 4;
	c |= (uint8_t)(b * 255.0); c <<= 4;

	return c;
}

