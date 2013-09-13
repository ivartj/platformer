#ifndef EVENT_H
#define EVENT_H

#define EVENT_KEY 1
#define EVENT_MOUSE 2

#include <stdint.h>

typedef union _event event;
typedef struct _keyevent keyevent;
typedef struct _mouseevent mouseevent;

struct _keyevent {
	int type;
	uint8_t code;
	uint16_t mod;
	uint16_t sym;
};

struct _mouseevent {
	int type;
	int x, y;
	uint8_t state;
};

union _event {
	int type;
	keyevent key;
	mouseevent mouse;
};

int event_wait(event *ev);
int event_poll(event *ev);

#endif
