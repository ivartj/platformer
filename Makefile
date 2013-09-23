LIBDIRS = container math system sdl gfx scene load

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
OBJLIBS = $(patsubst %,lib%.a,$(LIBDIRS))

CFLAGS = $(shell sdl-config --cflags) -include SDL.h $(patsubst %,-I $(shell pwd)/%,$(LIBDIRS)) -I $(shell pwd)/backend-headers $(shell pkg-config --cflags libpng) -g
LIBS = -lfreeimage $(shell sdl-config --libs) $(shell pkg-config --libs libpng) $(patsubst %,$(shell pwd)/lib%.a,${LIBDIRS}) -g

all: platformer demos

platformer: $(OBJS) $(OBJLIBS)
	$(CC) -o platformer $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o *.a platformer
	for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	$(MAKE) -C demos clean

lib%.a: %
	$(MAKE) CFLAGS="${CFLAGS}" -C $<
	cp $</$@ .

demos: demos-all

demos-all: $(OBJLIBS)
	$(MAKE) CFLAGS="${CFLAGS}" LIBS="${LIBS}" -C demos
