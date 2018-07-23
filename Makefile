ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

#OBJS specifies which files to compile as part of the project 
OBJS = ./*cpp
#HEADS = ./*.h

#CC specifies which compiler we're using 
CC = g++
#INCLUDE_PATHS specifies the additional include paths we'll need 
#INCLUDE_PATHS = -I ./SDL2-2.0.8/i686-w64-mingw32/include/

ifeq ($(detected_OS),Windows)
	obj_type = i386pe
else
	obj_type = elf_i386
endif

#LIBRARY_PATHS specifies the additional library paths we'll need 
LIBRARY_PATHS = -L ./lib

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
COMPILER_FLAGS = -w 

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -static-libgcc -static-libstdc++

BOOST = -lboost_filesystem-mgw63-mt-d-x32-1_67 -lboost_thread-mgw63-mt-d-x32-1_67 -lboost_regex-mgw63-mt-d-x32-1_67 -lboost_system-mgw63-mt-d-x32-1_67
 
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = FaPatcher.exe 

ext_sector:
	$(MAKE) all -C ./sections
	
_hooks: 
	$(MAKE) all OBJ_NAME=$(OBJ_NAME_) -C ./hooks
	
gpp_link:
	ld -T linker.ld -static -m  $(obj_type) build/ext_sector.o -o build/ext_sector.exe.tmp
	
rip_out_binary:
	objcopy -O binary -R .eh_fram build/ext_sector.exe.tmp build/ext_sector.bin

#This is the target that compiles our executable 
all : 
	$(CC) $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(BOOST) -o $(OBJ_NAME)