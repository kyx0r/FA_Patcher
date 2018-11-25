#include "global.h"
#include "typeDefs.h"
#include "moho.h"
#include <stdlib.h>

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

	typedef __attribute__((cdecl)) _DWORD *__Eval_Lua_Object(int *a1, const struct LuaStackObject *a2, LuaState *a3, int a4);
	__Eval_Lua_Object *Eval_Lua_Object = (__Eval_Lua_Object*)(_Eval_Lua_Object);

	typedef bool __Validate_IssueCommand(int a1, int a2, int a3);
	__Validate_IssueCommand *Validate_IssueCommand = (__Validate_IssueCommand*)(_Validate_IssueCommand);

	typedef __attribute__((thiscall)) int __Get_Lua_Coordinates_State(int a1, LuaState *a2, int a4, lua_State **a5, int a6);
	__Get_Lua_Coordinates_State *Get_Lua_Coordinates_State = (__Get_Lua_Coordinates_State*)(_Get_Lua_Coordinates_State);

	typedef int __Push_Coordinates(int result, int a2);
	__Push_Coordinates *Push_Coordinates = (__Push_Coordinates*)(_Push_Coordinates);

	typedef int __Moho_UNIT_IssueCommand(int a1, int a2, Moho__SSTICommandIssueData *a3, char a4);
	__Moho_UNIT_IssueCommand *Moho_UNIT_IssueCommand = (__Moho_UNIT_IssueCommand*)(_Moho_UNIT_IssueCommand);

	typedef int __Moho_SSTICommandIssueData_SSTICommandIssueData(_DWORD *_this, int a2);
	__Moho_SSTICommandIssueData_SSTICommandIssueData *Moho_SSTICommandIssueData_SSTICommandIssueData = (__Moho_SSTICommandIssueData_SSTICommandIssueData*)(_Moho_SSTICommandIssueData_SSTICommandIssueData);

	typedef int __attribute__((stdcall)) __Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData(int a1);
	__Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData *Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData =
	    (__Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData*)(_Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData);

	typedef uint32 *__LuaPlus_LuaState_PushNil(int a1);
	__LuaPlus_LuaState_PushNil *LuaPlus_LuaState_PushNil = (__LuaPlus_LuaState_PushNil*)(_LuaPlus_LuaState_PushNil);

	typedef _DWORD *__LuaPlus_LuaObject_PushStack(_DWORD *_this, _DWORD *a2, lua_State **a3);
	__LuaPlus_LuaObject_PushStack *LuaPlus_LuaObject_PushStack = (__LuaPlus_LuaObject_PushStack*)(_LuaPlus_LuaObject_PushStack);

	//extern functions:
	//MSVCR80.dll
	typedef __attribute__((cdecl)) int __memmove_s(void *Dst, size_t DstSize, const void *Src, size_t MaxCount);
	__memmove_s *memmove_s = (__memmove_s*)(_memmove_s);

	typedef __attribute__((cdecl)) int __lua_getglobaluserdata(int a1);
	__lua_getglobaluserdata *lua_getglobaluserdata = (__lua_getglobaluserdata*)(_lua_getglobaluserdata);

	//shsmp.dll
	typedef void __j_shi_delete_0(int a1);
	__j_shi_delete_0 *j_shi_delete_0 = (__j_shi_delete_0*)(_j_shi_delete_0);

	typedef int __j_shi_new_0(int a1);
	__j_shi_new_0 *j_shi_new_0 = (__j_shi_new_0*)(_j_shi_new_0);


	volatile int __Push_Coordinatesw(int r, int a2)
	{
		int result;
		__asm volatile(
		    "mov ecx, %[a2] \n\t" //point to the structure
		    "mov eax, %[r] \n\t"
		    "call %[func]\n\t"
		    : "=a" (result)
		    : [func] "m" (_Push_Coordinates), [r] "r" (r), [a2] "r" (a2)
		    : "memory"
		);
		return result;
	}

	volatile bool __Validate_IssueCommandw(int a1, int a2, int a3)
	{
		int result;
		__asm volatile(
		    "push %[a3] \n\t"
		    "push %[a2] \n\t"
		    "mov edi, %[a1] \n\t"
		    "call %[func]\n\t"
		    : "=a" (result)
		    : [func] "m" (_Validate_IssueCommand), [a1] "r" (a1), [a2] "r" (a2), [a3] "r" (a3)
		    : "memory"
		);
		return result;
	}

	volatile int __Get_Lua_Coordinates_Statew(int a1, LuaState *a2, int a4, lua_State **a5, int a6)
	{
		register int result asm("eax");
		__asm volatile(
		    "push %[a6] \n\t"
		    "push %[a5] \n\t"
		    "push %[a4] \n\t"
		    "push %[a2] \n\t"
		    //"mov st(0), %[a2] \n\t"
		    "mov edi, %[a1] \n\t"
		    "call %[func]\n\t"
		    : "=a" (result)
		    : [func] "m" (_Get_Lua_Coordinates_State), [a1] "r" (a1), [a2] "r" (a2), [a4] "r" (a4), [a5] "r" (a5), [a6] "r" (a6)
		    : "memory"
		);
		return result;
	}

	volatile int Moho_UNIT_IssueCommandw(int a1, int a2, Moho__SSTICommandIssueData *a3, int a4)
	{
		int result;
		__asm volatile(
		    "push %[a4] \n\t"
		    "push %[a3] \n\t"
		    "push %[a2] \n\t"
		    "mov edx,%[a1] \n\t"
		    "call %[func]\n\t"
		    : "=a" (result)
		    : [func] "m" (_Moho_UNIT_IssueCommand), [a1] "r" (a1), [a2] "r" (a2), [a3] "r" (a3), [a4] "r" (a4)
		    : "memory"
		);
		return result;

	}

};
