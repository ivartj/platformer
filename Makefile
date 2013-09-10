LIBDIRS = container

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
OBJLIBS = $(patsubst %,lib%.a,$(LIBDIRS))

CFLAGS = $(shell sdl-config --cflags)
LIBS = $(shell sdl-config --libs) $(OBJLIBS)

all: platformer

platformer: $(OBJS) $(OBJLIBS)
	$(CC) -o platformer $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o platformer
	for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

lib%.a: %
	$(MAKE) -C $<
	cp $</$@ .
