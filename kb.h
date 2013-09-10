#ifndef KB_H
#define KB_H

#include <stdint.h>

void setkeydown(uint16_t sym, int bool);
int iskeydown(uint16_t sym);

#endif
