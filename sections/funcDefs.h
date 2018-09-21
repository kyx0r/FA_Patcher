#include "global.h"
#include "typeDefs.h"
#include "moho.h"

//((int (*)(const char* fmt, ...))_Logf)("Hello World!"); //direct call example
/*  typedef void __Logf(const char* fmt, ...);
	 __Logf* Logf = reinterpret_cast<__Logf*>(_Logf);  */
	 
	 //required functions that have not been reversed yet. 	 
	 
class funcDefs
{
	public:
	
	typedef void __Logf(const char* fmt, ...);
	__Logf *Logf = (__Logf*)(_Logf);
	
	typedef void __LuaState__Error(LuaState *a1, char *a2, ...);
	__LuaState__Error *LuaState__Error = (__LuaState__Error*)(_LuaState__Error);
	
	typedef int __lua_gettop(int a1);
	__lua_gettop *lua_gettop = (__lua_gettop*)(_lua_gettop);
	
	typedef _DWORD *__Eval_Lua_Object(int *a1, const struct LuaStackObject *a2, LuaState *a3, int a4);
	__Eval_Lua_Object *Eval_Lua_Object = (__Eval_Lua_Object*)(_Eval_Lua_Object);
	
	typedef bool __Validate_IssueCommand(int a1, int a2, int a3);
	__Validate_IssueCommand *Validate_IssueCommand = (__Validate_IssueCommand*)(_Validate_IssueCommand);
	
	typedef void __Goto_Exception_Handler_Ntdll(int a1);
	__Goto_Exception_Handler_Ntdll *Goto_Exception_Handler_Ntdll = (__Goto_Exception_Handler_Ntdll*)(_Goto_Exception_Handler_Ntdll);
	
	typedef int __Get_Lua_Coordinates_State(int a1, LuaState *a2, int a4, lua_State **a5, signed int a6);
	__Get_Lua_Coordinates_State *Get_Lua_Coordinates_State = (__Get_Lua_Coordinates_State*)(_Get_Lua_Coordinates_State);
	
	typedef int __Push_Coordinates(int result, int a2);
	__Push_Coordinates *Push_Coordinates = (__Push_Coordinates*)(_Push_Coordinates);
	
	typedef int __Moho_UNIT_IssueCommand(int a1, int a2, Moho__SSTICommandIssueData *a3, char a4);
	__Moho_UNIT_IssueCommand *Moho_UNIT_IssueCommand = (__Moho_UNIT_IssueCommand*)(_Moho_UNIT_IssueCommand);
	
	typedef int __Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData(Moho__SSTICommandIssueData* a1);
	__Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData *Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData = 
	(__Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData*)(_Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData);
	
	typedef uint32 *__LuaPlus_LuaState_PushNil(int a1);
	__LuaPlus_LuaState_PushNil *LuaPlus_LuaState_PushNil = (__LuaPlus_LuaState_PushNil*)(_LuaPlus_LuaState_PushNil);
	
	typedef _DWORD *__LuaPlus_LuaObject_PushStack(_DWORD *_this, _DWORD *a2, lua_State **a3);
	__LuaPlus_LuaObject_PushStack *LuaPlus_LuaObject_PushStack = (__LuaPlus_LuaObject_PushStack*)(_LuaPlus_LuaObject_PushStack);

};
