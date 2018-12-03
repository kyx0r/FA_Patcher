ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

SRCS=$(wildcard *.cpp)

FASTOBJS=$(SRCS:.cpp=.o)

PROGS = $(patsubst %.cpp,../build/%.o,$(SRCS))

#CC specifies which compiler we're using 
CC = g++
#INCLUDE_PATHS specifies the additional include paths we'll need 
#INCLUDE_PATHS = -I ./SDL2-2.0.8/i686-w64-mingw32/include/

#LIBRARY_PATHS specifies the additional library paths we'll need 
#LIBRARY_PATHS = -L ./lib

LINKER_FLAGS = -I. -m32 -O0 -ffreestanding -fpermissive -fno-exceptions -fno-asynchronous-unwind-tables -c -w -Wextra -masm=intel -ffunction-sections -fdata-sections -Wl,--gc-sections

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
ifeq ($(detected_OS),Windows)
COMPILER_FLAGS = -fleading-underscore
endif

#LINKER_FLAGS specifies the libraries we're linking against 
#-ffunction-sections -fdata-sections

#-ffreestanding = freestanding enviroment

#-nostartfiles similar to -c but has linker enabled

ASSEMBLER_FLAGS =  -c -ffreestanding -fpermissive -fno-exceptions -fno-asynchronous-unwind-tables

#BOOST = -lboost_filesystem-mgw63-mt-d-x32-1_67 -lboost_thread-mgw63-mt-d-x32-1_67 -lboost_regex-mgw63-mt-d-x32-1_67 -lboost_system-mgw63-mt-d-x32-1_67

fast_compile: $(PROGS)
../build/%.o: %.cpp
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@ $<
 
all :
	$(CC) $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(BOOST) -o $(OBJ_NAME)
	
genAssembly:
	$(CC) $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(BOOST) -S $(OBJ_NAME)
	
assemble:
	$(CC) ./*s $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(ASSEMBLER_FLAGS) $(BOOST) -o $(OBJ_NAME)	
