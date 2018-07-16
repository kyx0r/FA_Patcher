#OBJS specifies which files to compile as part of the project 
OBJS = ./*cpp
HEADS = ./*.h

#CC specifies which compiler we're using 
CC = g++
#INCLUDE_PATHS specifies the additional include paths we'll need 
#INCLUDE_PATHS = -I ./SDL2-2.0.8/i686-w64-mingw32/include/

#LIBRARY_PATHS specifies the additional library paths we'll need 
#LIBRARY_PATHS = -L ./lib

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
ifeq ($(detected_OS),Windows)
COMPILER_FLAGS = -fleading-underscore
endif

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -I. -m32 -O0 -fno-exceptions -fno-asynchronous-unwind-tables -c

#BOOST = -lboost_filesystem-mgw63-mt-d-x32-1_67 -lboost_thread-mgw63-mt-d-x32-1_67 -lboost_regex-mgw63-mt-d-x32-1_67 -lboost_system-mgw63-mt-d-x32-1_67
 
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = ext_sector.o

#This is the target that compiles our executable 

all : 
	$(CC) $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(BOOST) -o $(OBJ_NAME)