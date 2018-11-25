
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

//#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
//#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "../pe_lib/pe_bliss.h"

using namespace std;
using namespace pe_bliss;
using namespace rang;

extern string make;
//Is debugging enabled?
//#define DEBUG

int debug_pause();

class Patcher
{
public:

	Patcher(const string& filename_in, const string& filename_out);

private:
	bool check_system();
	bool check_make();


};