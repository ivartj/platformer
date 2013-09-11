SRCS=$(wildcard *.c)
BINS=$(patsubst %.c,%,$(SRCS))

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm -f $(BINS)
