# FA_patch

Remake of Eximius patcher completely in C++ 
https://github.com/FAForever/FAPatch

PE Management Library used: (Modified)
https://github.com/mrexodia/portable-executable-library/tree/76d48e813764288774d6a3540731b23fffb8f4d5

Boost Library: (Modified)
https://www.boost.org/

Rang Library, for pretty console colors:
https://github.com/agauniyal/rang

Asmjit Library (Modified)
https://github.com/asmjit/asmjit

Custom embeded build of TCC compiler
https://github.com/TinyCC/tinycc

Thanks for those, so I don't have to reinvent the wheel. Though it kinda is what this project is altogether. 

Source code is kept here to be able to modify and adjust the libraries. 

# How To Build

- Cd into project directory and target make all or mingw32-make all

# Current External Dependencies

This project currently uses the following deps:

-None

# Dependencies for Windows

The dependencies used if you are compiling on Windows
is contained here on the repository.

One thing you need is minGw package. 
Follow instuctions, after should be good to go.
https://sourceforge.net/projects/mingw/?source=directory

Note: cmd build is no longer supported, so I reccomend to use
git bash shell https://gitforwindows.org/

# Dependencies for Unix-based systems

-GNU C compiler and GNU C++ compiler (32 bit compiler)

Tested on 64 bit linux machine, patcher compiles well.

(for every distro has its own install procedure, if not installed by default) 

If you are using a 64 bit compiler like me, then it might not have multilib support installed.

To get it read this: https://www.geeksforgeeks.org/compile-32-bit-program-64-bit-gcc-c-c/

(This is required for regular hooks and patches.) 

# Patches for FA can be found at: 

https://github.com/FAETHER/FA-Binary-Patches

