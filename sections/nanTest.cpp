#include "global_func_table.h"
#include <math.h>

bool GFT::nanTest(float *a1)
{
	return !isnan(*a1) && !isnan(a1[1]) && !isnan(a1[2]);
}