#include "global.h"

//((int (*)(const char* fmt, ...))_Logf)("Hello World!"); //direct call example
/*  typedef void __Logf(const char* fmt, ...);
	 __Logf* Logf = reinterpret_cast<__Logf*>(_Logf);  */

class funcDef: public global
{
	public:
	
	void Logf(const char* ss, ...)
	{
		((int (*)(const char* fmt, ...))_Logf)(ss);
	}
};
