
SRCS=$(wildcard ./base/*.cpp)
SRCS2=$(wildcard ./x86/*.cpp)
SRCS3=$(wildcard ./asmtk/*.cpp)

OBJS=$(SRCS:.cpp=.o)
OBJS2=$(SRCS2:.cpp=.o)
OBJS3=$(SRCS3:.cpp=.o)

LIBNAME = asmjit
LIBPATH = ../lib
CXXFLAGS = -O3 -w -fpermissive -fPIC -DPIC -I..

all: $(LIBPATH)/lib$(LIBNAME).a

clean:
	rm -f $(OBJS) $(OBJS2) $(OBJS3) lib$(LIBNAME).a
	rm -rf ../lib/lib$(LIBNAME).a

lib$(LIBNAME).a: $(OBJS) $(OBJS2) $(OBJS3)
	ar -cvr lib$(LIBNAME).a $(OBJS) $(OBJS2) $(OBJS3)
	ranlib lib$(LIBNAME).a

$(LIBPATH):
	mkdir -p ../lib

$(LIBPATH)/lib$(LIBNAME).a: lib$(LIBNAME).a $(LIBPATH)
	cp -d lib$(LIBNAME).a ../lib