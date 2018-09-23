
SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

LIBNAME = filesystem
LIBPATH = ../../lib
CXXFLAGS = -O2 -Wall -fPIC -DPIC -I..

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJS) lib$(LIBNAME).a
	rm -rf ../../lib/$(LIBNAME).a

lib$(LIBNAME).a: $(OBJS)
	ar -cvr lib$(LIBNAME).a $(OBJS)
	ranlib lib$(LIBNAME).a

$(LIBPATH):
	mkdir -p ../../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../../lib