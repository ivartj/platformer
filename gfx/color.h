#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef uint32_t color; 

color rgba(double r, double g, double b, double a);
color rgb(double r, double g, double b);
void getrgb(color c, double *r, double *g, double *b);

#endif
