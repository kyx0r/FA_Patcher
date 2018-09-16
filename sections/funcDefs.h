#include "global.h"
#include "typeDefs.h"
#include "moho.h"

//((int (*)(const char* fmt, ...))_Logf)("Hello World!"); //direct call example
/*  typedef void __Logf(const char* fmt, ...);
	 __Logf* Logf = reinterpret_cast<__Logf*>(_Logf);  */
	 
	 //required functions that have not been reversed yet. 	 
	 
class funcDefs: public global
{
	public:
	
	void Logf(const char* ss, ...)
	{
		((int (*)(const char* fmt, ...))_Logf)(ss);
	}
	
	void LuaState__Error(LuaState *a1, char *a2, ...)
	{
		((int (*)(LuaState *a1, char *a2, ...))_LuaState__Error)(a1,a2);
	}
	
	int lua_gettop(int a1)
	{
		return ((int (*)(int a1))_lua_gettop)(a1);
	}
	
	uint32* Eval_Lua_Object(int *a1, LuaStackObject *a2, LuaState *a3, int a4)
	{
		return ((uint32* (*)(int *a1, LuaStackObject *a2, LuaState *a3, int a4))_Eval_Lua_Object)(a1,a2,a3,a4);
	}
	
	bool Validate_IssueCommand(int a1, int a2, int a3)
	{
		return ((bool* (*)(int a1, int a2, int a3))_Validate_IssueCommand)(a1,a2,a3);
	}
	
	void Goto_Exception_Handler_Ntdll(int a1)
	{
		((void (*)(int a1))_Goto_Exception_Handler_Ntdll)(a1);
	}
	
	int Get_Lua_Coordinates_State(int a1, LuaState *a2, int a4, lua_State **a5, signed int a6)
	{
		return ((int (*)(int a1, LuaState *a2, int a4, lua_State **a5, signed int a6))_Get_Lua_Coordinates_State)(a1,a2,a4,a5,a6);
	}
	
	int Push_Coordinates(int result, int a2)
	{
		return ((int (*)(int result, int a2))_Push_Coordinates)(result, a2);
	}
	
	int Moho_UNIT_IssueCommand(int a1, int a2, Moho__SSTICommandIssueData *a3, char a4)
	{
		return ((int (*)(int a1, int a2, Moho__SSTICommandIssueData *a3, char a4))_Moho_UNIT_IssueCommand)(a1,a2,a3,a4);
	}
	
	int Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData(Moho__SSTICommandIssueData* a1)
	{
		return ((int (*)(Moho__SSTICommandIssueData* a1))_Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData)(a1);
	}
	
	uint32* LuaPlus_LuaState_PushNil(int a1)
	{
		return ((uint32 (*)(int a1))_LuaPlus_LuaState_PushNil)(a1);
	}
	
	_DWORD* LuaPlus_LuaObject_PushStack(_DWORD *_this, _DWORD *a2, lua_State **a3)
	{
		return ((_DWORD (*)(_DWORD *_this, _DWORD *a2, lua_State **a3))_LuaPlus_LuaObject_PushStack)(_this,a2,a3);
	}
	
};
