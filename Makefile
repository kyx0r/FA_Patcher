ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname -s 2>/dev/null || echo not')
endif

ifeq ($(detected_OS),Windows)
	obj_type = i386pe
	mkdir = mkdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	rm = $(wordlist 2,65535,$(foreach FILE,$(subst /,\,$(1)),& del $(FILE) > nul 2>&1)) || (exit 0)
	rmdir = rmdir $(subst /,\,$(1)) > nul 2>&1 || (exit 0)
	echo = echo $(1)
else
	obj_type = elf_i386
	mkdir = mkdir -p $(1)
	rm = rm $(1) > /dev/null 2>&1 || true
	rmdir = rmdir $(1) > /dev/null 2>&1 || true
	echo = echo "$(1)"	
endif

OBJS = ./*cpp
#HEADS = ./*.h
CC = g++

#LIBRARY_PATHS specifies the additional library paths we'll need 
LIBRARY_PATHS = -L ./lib

align_size = 0x100

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# -Wl,-subsystem,windows gets rid of the console window 
COMPILER_FLAGS = -w -Dalign_size

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -static-libgcc -static-libstdc++

BOOST = -lboost_filesystem-mgw63-mt-d-x32-1_67 -lboost_thread-mgw63-mt-d-x32-1_67 -lboost_regex-mgw63-mt-d-x32-1_67 -lboost_system-mgw63-mt-d-x32-1_67

PELIB = -lpebliss
 
#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = FaPatcher.exe 

#-oformat -Ttext=0x006B8FB9
#echo align_size = $(align_size)';' > Env.ld

align:
	echo align_size = $(align_size)';' > Env.ld
	
peLib:
	$(MAKE) all -C ./pe_lib
	
directories:
	$(call mkdir, /build)

ext_sector:
	$(MAKE) all -C ./sections
	
_hooks:
	$(MAKE) all OBJ_NAME=$(OBJ_NAME_) OBJS=$(OBJS) -C ./hooks
	
ext_gpp_link:
	ld -T ./linker/sectionLinker.ld -static -m  $(obj_type) $(PRIME_NAME) -o $(TMP_NAME)

hook_gpp_link:
	$(call echo, align_size = $(align_size)';' > Env.ld)
	ld -T ./linker/hookLinker.ld -static -m  $(obj_type) $(PRIME_NAME) -o $(TMP_NAME)
	
rip_out_binary:
	objcopy --strip-all -O binary -R .eh_fram $(TMP_NAME) $(PRIME_NAME)

#This is the target that compiles our executable 
all : peLib
	$(CC) $(OBJS) $(HEADS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(BOOST) $(PELIB) -o $(OBJ_NAME)
	./FaPatcher