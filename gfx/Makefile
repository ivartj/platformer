LIBNAME = gfx

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

all: lib${LIBNAME}.a

lib${LIBNAME}.a: $(OBJS)
	$(AR) -rc lib${LIBNAME}.a $(OBJS)

%.o: %.c
	$(CC) -c ${CFLAGS} $<

clean:
	rm -f *.o lib${LIBNAME}.a
