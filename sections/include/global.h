#pragma once
//LuaPlus
const int LuaState__CastState = 0x90A510; // LuaState* (lua_state*)
const int _LuaState__Error = 0x90C1D0; // (LuaState*, fmt, args...)
const int _CastState_LuaState_LuaPlus__SAPAV12_PAUlua_State___Z = 0x90A510;
const int __0LuaObject_LuaPlus__QAE_ABVLuaStackObject_1__Z = 0x908A70;
const int __1LuaObject_LuaPlus__QAE_XZ = 0x9075D0;
const int LuaObject_GetString = 0x907A90;

//	struct LuaStackObject
//	{
//		// 0x8 bytes
//		LuaState* state;
//		int stack_index;
//	};

const int LuaStackObject__GetBoolean = 0x415560;

const int lua_pushnumber = 0x90CD40; //(lua_state*, float)
const int _lua_pushbool = 0x90CF80; //(lua_state*, bool)

const int _AssignClientIndex_CLobby_Moho__AAEXHVStrArg_gpg__AAIAAH_Z = 0x7C4E80;
const int _AssignCommandSource_CLobby_Moho__AAEIHHAAV_$vector_USSTICommandSource_Moho__V_$allocator_USSTICommandSource_Moho___std___std__AAI_ = 0x7C4F60;

const int lua_cast_CLobby_ = 0x7CB7E0;

const int Moho__NET_MakeNATTraversal_ = 0x7BC5F0; //(LuaObject* rvo, LuaState*)
const int LuaObject_PushStack = 0x907D80; //(LuaStackObject* rvo, LuaState*)
const int _Eval_Lua_Object = 0x6EEE40;
const int _LuaPlus_LuaState_PushNil = 0x0090CD00;
const int _LuaPlus_LuaObject_PushStack = 0x00907D80;
const int _lua_getglobaluserdata = 0x00924050;

// Returned object is probably an iterator of sorts_
// It is 0x10 bytes.
const int Moho__BVSet_Add = 0x6E5660; //void* __stdcall (void* this<ecx>, void* rvo<ebx>, int setItem)
const int Moho__Set__Add = 0x4036A0; //void *__stdcall (void *this<edi>, void *rvo, int setItem<esi>)


// Utils
const int _lua_tostring__YAPBDPAUlua_State__H_Z = 0x90CA90;
const int _lua_tostring = 0x90CA90;
const int _lua_tonumber = 0x90C9F0;
const int _lua_toboolean = 0x90CA40;
const int __imp_atoi = 0xA835CE;

const int __ftol2_sse = 0xA89CC0;

const int _lua_gettop = 0x90C590; //(lua_state*)

const int __stricmp = 0xAA549E; //int (const char*, const char*)

//const int Logf = 0x937CB0;
const int _Logf = 0x937CB0;
const int _Validate_IssueCommand = 0x006EECF0;
const int _j_shi_delete_0 = 0x00A82542;
const int _j_shi_new_0 = 0x00A82130;
const int _Get_Lua_Coordinates_State = 0x006EEF60;
const int _Push_Coordinates = 0x005E27D0;
const int _Moho_UNIT_IssueCommand = 0x006F12C0;
const int _Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData = 0x0057ABB0;

// Globals

const int g_STIDriver = 0x10C4F50;
const int g_SWldSessionInfo = 0x10C4F58;
const int g_CWldSession = 0x10A6470;
const int _g_CWldSession = 0x10A6470;

const int _g_Sim = 0x10A63F0;

// String const
const int s_FACTORY = 0xE19824;
const int s_EXPERIMENTAL = 0xE204B8;

// Int const

//Adress const
const int _CannotQueCommandInConstruct = 0x006EFB0E;
const int _CanQueCommandInConstruct = 0x006EFAF8;
const int _NeitherInCategoryInConstruct = 0x006EFACE;
const int _EndCalculateNoRushTimerVariable = 0x006FF3D6;

// c Symbols

const int _CheckCategory = 	0x00405550;
const int _CheckCategory_sub_func = 0x004059E0;
const int _GetCatCmpResult = 0x0067B050;
const int _exit_STAYONWATSUR_check = 0x0062ADEE;
const int _exit_STAYONWATSUR_NoMatch = 0x0062ADEC;
const int _Moho_SSTICommandIssueData_SSTICommandIssueData = 0x00552550;

//extern functions:
//MSVCR80.dll
const int _memmove_s = 0x00A824E7;
