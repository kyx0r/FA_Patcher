
#include "moho.h"
#include "funcDef.h"
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

funcDef fd;

void print_hello_world()
{	
	fd.Logf("Hello World!");
}


/* void Thread__()
{
	hw HW;
	boost::thread worker(boost::bind(&hw::print_hello_world, &HW));
	worker.join();
}

int main(){} */