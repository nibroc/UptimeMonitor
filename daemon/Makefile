CC=gcc
CXX=g++
RM=rm -f
CXXFLAGS=-Wall -pedantic -O3
LDLIBS=-lcurl -lrt

SRCS=uptimed.cpp post.cpp procparse/hostname.cpp procparse/loadavg.cpp procparse/meminfo.cpp procparse/uptime.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: uptimed

uptimed: $(OBJS)
	g++ $(LDFLAGS) -o uptimed $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
