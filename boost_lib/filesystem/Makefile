
SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

LIBNAME = filesystem
LIBPATH = ../../lib
CXXFLAGS = -O3 -w -fpermissive -fPIC -DPIC -I..

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJS) lib$(LIBNAME).a
	rm -rf ../../lib/lib$(LIBNAME).a

lib$(LIBNAME).a: $(OBJS)
	ar -cvr lib$(LIBNAME).a $(OBJS)
	ranlib lib$(LIBNAME).a

.PHONY $(LIBPATH):
	mkdir -p ../../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../../lib