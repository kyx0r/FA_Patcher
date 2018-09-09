
#include "moho.h"
#include "funcDefs.h"
#include <stdlib.h>
#include "global_func_table.h"
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

funcDefs fd;
GFT Gft;

void print_hello_world()
{	
	fd.Logf("Hello World!");
	Gft.IssueMove(0x1233545,1244);
}


/* void Thread__()
{
	hw HW;
	boost::thread worker(boost::bind(&hw::print_hello_world, &HW));
	worker.join();
}

int main(){} */