
SRCS=$(wildcard *.cpp)

OBJS=$(SRCS:.cpp=.o)

LIBNAME = patcher
LIBPATH = ../lib
OBJ_NAME = FaPatcher.exe

CXXFLAGS = -O3 -w -fpermissive -fPIC -Wextra -masm=intel -I ../boost_lib -I .. -DOBJ_NAME='"$(OBJ_NAME)"'

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