
#pragma once

#include "rang.hpp"

#include <iostream>
//#include <string>
#include <cstring>
#include <limits>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <assert.h>
#include <fstream>
#include <csetjmp>

//#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
//#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "../pe_lib/pe_bliss.h"

using namespace std;
using namespace pe_bliss;
using namespace rang;

extern string make;
extern bool patcher_error;
extern string uname;
extern jmp_buf jump_buffer;
//Is debugging enabled?
//#define DEBUG

[[noreturn]] void debug_pause();

class Patcher
{
public:

	Patcher(string filename_in = "", string filename_out = "");
	void load_uname();

private:
	bool check_system();
	bool check_make();
	bool check_gcc();


};