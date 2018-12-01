
SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

LIBNAME = patcher
LIBPATH = ../lib
CXXFLAGS = -O3 -w -fpermissive -fPIC -DPIC -I ../boost_lib -I ..

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJS) lib$(LIBNAME).a
	rm -rf $(LIBPATH)/lib$(LIBNAME).a

lib$(LIBNAME).a: $(OBJS)
	ar -cvr lib$(LIBNAME).a $(OBJS)
	ranlib lib$(LIBNAME).a

.PHONY $(LIBPATH):
	mkdir -p ../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../lib