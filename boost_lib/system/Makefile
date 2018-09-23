
SRCS=$(wildcard *.cpp)

STRIP = $(SRCS:../boost/system/%=%)
OBJS=$(SRCS:.cpp=.o)

head = ../boost/cerrno.hpp
heads = ../boost/system/*.hpp

LIBNAME = system
LIBPATH = ../../lib
CXXFLAGS = -O0 -m32 -Wall -g -march=i686  -fno-inline -fPIC -DPIC -I.. -DBOOST_ALL_NO_LIB=1 -DBOOST_SYSTEM_DYN_LINK=1 

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJS) lib$(LIBNAME).a
	rm -rf ../../lib/lib$(LIBNAME).a

lib$(LIBNAME).a: $(OBJS) 
	ar -cvr lib$(LIBNAME).a $(OBJS) 
	ranlib lib$(LIBNAME).a

$(LIBPATH):
	mkdir -p ../../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../../lib