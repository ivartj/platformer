SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

all: libcontainer.a

libcontainer.a: $(OBJS)
	$(AR) -rc libcontainer.a $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o libcontainer.a
