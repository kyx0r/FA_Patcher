
SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

LIBNAME = patcher
LIBPATH = ../lib
CXXFLAGS = -O3 -Wall -fPIC -DPIC -I ../boost_lib

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJS) lib$(LIBNAME).a
	rm -rf $(LIBPATH)/lib$(LIBNAME).a

lib$(LIBNAME).a: $(OBJS)
	ar -cvr lib$(LIBNAME).a $(OBJS)
	ranlib lib$(LIBNAME).a

$(LIBPATH):
	mkdir -p ../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../lib