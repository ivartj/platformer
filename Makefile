LIBDIRS = container math system sdl gfx

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
OBJLIBS = $(patsubst %,lib%.a,$(LIBDIRS))

CFLAGS = $(shell sdl-config --cflags) $(patsubst %,-I $(shell pwd)/%,$(LIBDIRS)) -I $(shell pwd)/backend-headers
LIBS = $(shell sdl-config --libs) $(patsubst %,$(shell pwd)/lib%.a,${LIBDIRS})

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

demos: $(OBJLIBS)
	$(MAKE) CFLAGS="${CFLAGS}" LIBS="${LIBS}" -C $@
