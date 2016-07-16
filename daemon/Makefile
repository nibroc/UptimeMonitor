CFLAGS=-Wall -Wextra -pedantic -std=gnu99
LDLIBS=-lcurl
RM=rm -f

SRCS=uptimed.c notifier.c string_buffer.c procparse/hostname.c procparse/loadavg.c procparse/meminfo.c procparse/uptime.c procparse/procparse.c
OBJS=$(subst .c,.o,$(SRCS))

all: release

release: CFLAGS += -O2
release: build

debug: CFLAGS += -O0 -g
debug: LDFLAGS += -O0 -g
debug: build

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

docs:
	doxygen doxygen.conf

include .depend
