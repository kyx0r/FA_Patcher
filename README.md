# FA_patch

Remake of Eximius patcher completely in C++ 
https://github.com/FAForever/FAPatch

PE Management Library used: 
https://github.com/mrexodia/portable-executable-library/tree/76d48e813764288774d6a3540731b23fffb8f4d5

Thanks for that, so I don't have to reinvent the wheel. Though it kinda is what this project is altogether. 

Source code is kept here to be able to modify and adjust the library. 

Boost Library:
https://www.boost.org/

Rang library, for pretty console colors:
https://github.com/agauniyal/rang

# How To Build

- Cd into project directory and target make all

# Current External Dependencies

This project currently uses the following deps:

-None

# Dependencies for Windows

The dependencies used if you are compiling on Windows
is contained here on the repository.

One thing you need is minGw package. 
Follow instuctions, after should be good to go.
https://sourceforge.net/projects/mingw/?source=directory

# Dependencies for Unix-based systems

-GNU C compiler and GNU C++ compiler (32 bit compiler)

Tested on 64 bit linux machine, patcher compiles well.

(for every distro has its own install procedure, if not installed by default) 

# Notes

Mingw linker has some flags that I use and they are not present in linker on linux.

So hooks need to be retargeted with different build flags, and object files formats. 

Some day I will implement full linux support. As of now, only the patcher can compile.
