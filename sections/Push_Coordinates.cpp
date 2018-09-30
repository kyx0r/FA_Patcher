#include "include/global_func_table.h"
#include "include/funcDefs.h"

//funcDefs FD; 

//----- (005E27D0) --------------------------------------------------------
//int __usercall sub_5E27D0@<eax>(int result@<eax>, int a2@<ecx>)
int GFT::Push_Coordinates(int result, int a2)
{
  int v2; // edx
  int v3; // ecx
  int v4; // ecx

  if ( *(_DWORD *)a2 == 1 )
  {
    v2 = *(_DWORD *)(a2 + 4);
    if ( v2 && v2 != 4 )
    {
      v3 = *(_DWORD *)(a2 + 4);
      if ( v3 )
        v4 = *(_DWORD *)(v3 - 4 + 104);
      else
    //    v4 = MEMORY[0x68];
      *(_DWORD *)result = 1;
      *(_DWORD *)(result + 4) = v4;
      goto LABEL_13;
    }
    *(_DWORD *)result = 1;
  }
  else
  {
    if ( *(_DWORD *)a2 == 2 )
    {
      *(_DWORD *)result = 2;
      *(_DWORD *)(result + 4) = -268435456;
      *(float *)(result + 8) = *(float *)(a2 + 12);
      *(float *)(result + 12) = *(float *)(a2 + 16);
      *(float *)(result + 16) = *(float *)(a2 + 20);
      return result;
    }
    *(_DWORD *)result = 0;
  }
  *(_DWORD *)(result + 4) = -268435456;
LABEL_13:
  //*(_QWORD *)(result + 12) = 0i64;
  *(_DWORD *)(result + 8) = 0;
  return result;
}