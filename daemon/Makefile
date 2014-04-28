CXXFLAGS=-Wall -pedantic -O3

LDLIBS=-lcurl -lrt

RM=rm -f

PROCPARSE_SRCS=procparse/hostname.c procparse/loadavg.c procparse/meminfo.c procparse/uptime.c
PROCPARSE_OBJS=$(subst .c,.o,$(PROCPARSE_SRCS))

DAEMON_SRCS=uptimed.cpp post.cpp
DAEMON_OBJS=$(subst .cpp,.o,$(DAEMON_SRCS))

SRCS=$(PROCPARSE_SRCS) $(DAEMON_SRCS)
OBJS=$(PROCPARSE_OBJS) $(DAEMON_OBJS)

all: uptimed

uptimed: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o uptimed $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
