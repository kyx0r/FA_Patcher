// emit reversed functions for later use. 
#include "typeDefs.h"

class GFT
{
	public:
	int IssueMove(int *a1, double a2);
	bool nanTest(float *a1);
	_DWORD* LuaPlus_LuaObject_LuaObject(_DWORD *_this);
	_DWORD* Moho_SSTICommandIssueData_SSTICommandIssueData(_DWORD* _this, int a2);
	int sub_5796A0(_DWORD *a1);
	
	
	
	
	
	
	
	
	
	
	//----- (00924050) --------------------------------------------------------
	int lua_getglobaluserdata(int a1)
	{
		return *(_DWORD *)(*(_DWORD *)(a1 + 16) + 328);
	}
	//int lua_gettop(int &a1);
	
};