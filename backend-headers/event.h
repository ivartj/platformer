#ifndef EVENT_H
#define EVENT_H

#define EVENT_KB 1

#include <stdint.h>

typedef union _event event;
typedef struct _kbevent kbevent;

struct _kbevent {
	int type;
	uint8_t code;
	uint16_t mod;
	uint16_t sym;
};

union _event {
	int type;
	kbevent kb;
};

int event_wait(event *ev);
int event_poll(event *ev);

#endif
