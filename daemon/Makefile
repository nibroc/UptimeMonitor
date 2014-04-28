CFLAGS=-Wall -Wextra -pedantic -std=gnu11
LDLIBS=-lcurl -lrt
RM=rm -f

SRCS=uptimed.c post.c procparse/hostname.c procparse/loadavg.c procparse/meminfo.c procparse/uptime.c
OBJS=$(subst .c,.o,$(SRCS))

all: build

build: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o uptimed $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;

clean:
	$(RM) uptimed $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
