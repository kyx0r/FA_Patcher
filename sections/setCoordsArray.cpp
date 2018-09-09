#include "global_func_table.h"
#include "funcDefs.h"

//----- (009072A0) --------------------------------------------------------
_DWORD* GFT::LuaPlus_LuaObject_LuaObject(_DWORD *_this)
{
  _DWORD *result; // eax

  result = _this;
  *_this = 0;
  _this[1] = 0;
  _this[2] = 0;
  _this[3] = 0;
  return result;
}

/* //----- (0057ABB0) --------------------------------------------------------
int __stdcall sub_57ABB0(int a1)
{
  int result; // eax
  int *v2; // eax
  int v3; // ecx

  sub_9075D0((_DWORD **)(a1 + 120));
  result = *(_DWORD *)(a1 + 88);
  if ( result == *(_DWORD *)(a1 + 100) )
  {
    *(_DWORD *)(a1 + 92) = result;
  }
  else
  {
    sub_A82542(*(_DWORD *)(a1 + 88));
    v2 = *(int **)(a1 + 100);
    *(_DWORD *)(a1 + 88) = v2;
    result = *v2;
    v3 = *(_DWORD *)(a1 + 88);
    *(_DWORD *)(a1 + 96) = result;
    *(_DWORD *)(a1 + 92) = v3;
  }
  return result;
} */

//----- (005796A0) --------------------------------------------------------
int GFT::sub_5796A0(_DWORD *a1)
{
  _DWORD *v1; // eax
  int result; // eax

  if ( a1[2] != a1[5] )
  {
	funcDefs Fd; 
    Fd.Goto_Exception_Handler_Ntdll(a1[2]);
    v1 = (_DWORD *)a1[5];
    a1[2] = v1;
    a1[4] = *v1;
  }
  a1[3] = a1[2];
  result = a1[1];
  *(_DWORD *)(*a1 + 4) = result;
  *(_DWORD *)a1[1] = *a1;
  a1[1] = a1;
  *a1 = a1;
  return result;
}

//----- (00552550) --------------------------------------------------------
_DWORD* GFT::Moho_SSTICommandIssueData_SSTICommandIssueData(_DWORD *_this, int a2)
{
  _DWORD *v2; // esi

  v2 = _this;
  *_this = -1; 
  _this[1] = -1;
  _this[2] = -1;
  _this[3] = a2;
  _this[4] = 0;
  _this[5] = -268435456;
  _this[6] = 0;
  _this[7] = 0;
  _this[8] = 0;
  _this[9] = 0;
  _this[10] = -268435456;
  _this[11] = 0;
  _this[12] = 0;
  _this[13] = 0;
  _this[14] = -1;
  _this[15] = 1065353216;
  _this[16] = 0;
  _this[17] = 0;
  _this[18] = 0;
  _this[19] = 1065353216;
  _this[20] = 0;
  v2[22] = (int)v2 + 26;
  v2[23] = (int)v2 + 26;
  v2[24] = (int)v2 + 28;
  v2[25] = (int)v2 + 26;
  _this[28] = 1;
  _this[29] = 1;
  LuaPlus_LuaObject_LuaObject(_this + 30);
  return v2;
}