
#include "moho.h"
#include "global.h"
#include <stdlib.h>
/* #include <iostream>
#include <cstring>
#include <limits>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <boost/thread/thread.hpp> */

// New unit categories.
const char* sCQUEMOV = "CQUEMOV";
const char* sSTAYONWATSUR = "STAYONWATSUR";
// ForgedAlliance.exe addresses.

/* typedef void __Logf(const char* fmt, ...);
__Logf* Logf = (__Logf*)_Logf; */ //relative call

//asm(".text 0x128D000");

void print_hello_world()
{	
	((int (*)(const char* fmt, ...))_Logf)("Hello World!"); 
}


/* void Thread__()
{
	hw HW;
	boost::thread worker(boost::bind(&hw::print_hello_world, &HW));
	worker.join();
}

int main(){} */