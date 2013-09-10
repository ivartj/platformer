CFLAGS = $(shell sdl-config --cflags)
LIBS = $(shell sdl-config --libs)
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

all: platformer

platformer: $(OBJS)
	$(CC) -o platformer $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $^

clean:
	rm -f *.o platformer
