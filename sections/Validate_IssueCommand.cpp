#include "include/global_func_table.h"
#include "include/funcDefs.h"

funcDefs FD; 

//_DWORD *__usercall sub_581270@<eax>(_DWORD *result@<eax>, _DWORD *a2@<edx>, _DWORD *a3@<ecx>)
_DWORD* GFT::cmp_pointers(_DWORD *result, _DWORD *a2, _DWORD *a3)
{
  for ( ; a3 != a2; ++result )
  {
    if ( result )
    {
      *result = *a3;
      ++a3;
    }
  }
  return result;
}

//int __fastcall sub_5811A0(_DWORD *a1, int a2, int a3, _DWORD *a4, _DWORD *a5)
__attribute__((fastcall)) int GFT::sub_5811A0(_DWORD *a1, int a2, int a3, _DWORD *a4, _DWORD *a5)
{
  _DWORD *v5; // edi
  int v6; // esi
  _DWORD *v7; // eax
  _DWORD *v8; // ebx
  _DWORD *v9; // ecx
  _DWORD *v10; // eax
  _DWORD *v11; // eax
  _DWORD *v12; // edi
  _DWORD *i; // eax
  _DWORD *v14; // eax
  int result; // eax
  int v16; // [esp+0h] [ebp-20h]
  _DWORD *v17; // [esp+Ch] [ebp-14h]
  int *v18; // [esp+10h] [ebp-10h]
  int v19; // [esp+1Ch] [ebp-4h]

  v5 = a1;
  v18 = &v16;
  v6 = a2;
  v7 = (_DWORD *)FD.j_shi_new_0(4 * a3);
  v8 = v7;
  v19 = 0;
  v9 = *(_DWORD **)v6;
  v17 = v7;
  v10 = cmp_pointers(v7, v5, v9);
  v11 = cmp_pointers(v10, a5, a4);
  v12 = cmp_pointers(v11, *(_DWORD **)(v6 + 4), v5);
  for ( i = *(_DWORD **)v6; i != *(_DWORD **)(v6 + 4); ++i )
    ;
  v14 = *(_DWORD **)(v6 + 12);
  if ( *(_DWORD **)v6 == v14 )
    *v14 = *(_DWORD *)(v6 + 8);
  else
    FD.j_shi_delete_0(*(_DWORD **)v6);
  result = a3;
  *(_DWORD *)(v6 + 4) = v12;
  *(_DWORD *)v6 = v8;
  *(_DWORD *)(v6 + 8) = &v8[a3];
  return result;
}

//_DWORD *__stdcall sub_57FE30(int a1, void *a2, void *Src, int a4)
_DWORD* GFT::sub_57FE30(int a1, void *a2, void *Src, int a4)
{
  _DWORD *v4; // esi
  int v5; // ebx
  unsigned int v6; // eax
  unsigned int v7; // ecx
  unsigned int v8; // ecx
  _DWORD *result; // eax
  int v10; // eax
  _DWORD *v11; // eax
  int v12; // ebx
  int v13; // edi
  int v14; // edi

  v4 = *(_DWORD *)(a1 + 4);
  v5 = (a4 - (signed int)Src) >> 2;
  v6 = v5 + ((*(_DWORD *)(a1 + 4) - *(_DWORD *)a1) >> 2);
  v7 = (*(_DWORD *)(a1 + 8) - *(_DWORD *)a1) >> 2;
  if ( v6 <= v7 )
  {
    v10 = 4 * v5;
    if ( (char *)a2 + 4 * v5 <= v4 )
    {
      v13 = (int)&v4[-v10];
      result = cmp_pointers(v4, v4, &v4[-v10]);
      v14 = (v13 - (signed int)a2) >> 2;
      *(_DWORD *)(a1 + 4) = result;
      if ( v14 > 0 )
        result = (_DWORD *)FD.memmove_s(&v4[-4 * v14], 4 * v14, a2, 4 * v14);
      if ( v5 > 0 )
        result = (_DWORD *)FD.memmove_s(a2, 4 * v5, Src, 4 * v5);
    }
    else
    {
      v11 = cmp_pointers(v4, (_DWORD *)a4, (_DWORD *)Src + ((v4 - (_DWORD *)a2) >> 2));
      *(_DWORD *)(a1 + 4) = v11;
      result = cmp_pointers(v11, v4, a2);
      v12 = 4 * ((v4 - (_DWORD *)a2) >> 2) >> 2;
      *(_DWORD *)(a1 + 4) = result;
      if ( v12 > 0 )
        result = (_DWORD *)FD.memmove_s(&v4[-4 * v12], 4 * v12, Src, 4 * v12);
    }
  }
  else
  {
    v8 = 2 * v7;
    if ( v6 < v8 )
      v6 = v8;
    result = (_DWORD *)sub_5811A0(a2, a1, v6, Src, (_DWORD *)a4);
  }
  return result;
}

//----- (10174AD0) --------------------------------------------------------
//----- (00582EB0) --------------------------------------------------------
//int __usercall sub_582EB0@<eax>(int *a1@<eax>, int a2@<edx>, int a3@<ecx>)
int GFT::sub_582EB0(int *a1, int a2, int a3)
{
  int v3; // esi
  int v4; // ecx
  int v5; // edi
  int v6; // eax
  int v7; // edx

  v3 = a2;
  v4 = (a3 - a2) >> 2;
  if ( v4 > 0 )
  {
    v5 = *a1;
    do
    {
      v6 = v4 / 2;
      if ( v5 )
        v7 = v5 + 8;
      else
        v7 = 0;
      if ( *(_DWORD *)(*(_DWORD *)(v3 + 4 * v6) + 104) >= *(_DWORD *)(v7 + 104) )
      {
        v4 /= 2;
      }
      else
      {
        v3 += 4 * v6 + 4;
        v4 += -1 - v6;
      }
    }
    while ( v4 > 0 );
  }
  return v3;
}

//int __userpurge sub_57DDD0@<eax>(int a1@<eax>, int a2@<ebx>, int Src, int a4)
int GFT::sub_57DDD0(int a1, int a2, int Src, int a4)
{
  int v4; // esi
  _DWORD *v5; // ebp
  int v6; // edi
  _DWORD *v7; // eax
  int v8; // edx
  int result; // eax
  int v10; // esi

  v4 = *(_DWORD *)(a1 + 12);
  v5 = (_DWORD *)(a1 + 8);
  v6 = *(_DWORD *)(a1 + 8);
  v7 = (_DWORD *)sub_582EB0(&Src, v6, v4);
  if ( v7 == (_DWORD *)v4 || (!Src ? (v8 = 0) : (v8 = Src + 8), *v7 != v8) )
  {
    v10 = ((signed int)v7 - v6) >> 2;
    if ( Src )
      Src += 8;
    else
      Src = 0;
    sub_57FE30((int)v5, v7, &Src, (int)&a4);
    *(_DWORD *)a2 = *v5 + 4 * v10;
    *(_BYTE *)(a2 + 4) = 1;
    result = a2;
  }
  else
  {
    *(_DWORD *)a2 = v7;
    *(_BYTE *)(a2 + 4) = 0;
    result = a2;
  }
  return result;
}

//----- (006EECF0) --------------------------------------------------------
//BOOL __usercall sub_6EECF0@<eax>(int a1@<edi>, int a2, int a3)
bool GFT::Validate_IssueCommand(int a1, int a2, int a3)
{
  _DWORD *i; // esi
  int v4; // ecx
  int v5; // eax
  int v6; // ecx
  int v7; // ecx
  int v8; // eax
  int v10; // [esp+0h] [ebp-1Ch]
  char v11; // [esp+10h] [ebp-Ch]

  for ( i = *(_DWORD **)(a1 + 8); i != *(_DWORD **)(a1 + 12); ++i )
  {
    if ( *i )
      v4 = *i - 8;
    else
      v4 = 0;
    if ( a3 & *(_DWORD *)((*(int (**)(void))(*(_DWORD *)v4 + 64))() + 96) )
    {
      if ( !(a3 & 0x19) && !(a3 & 0x2000)
        || (!*i ? (v5 = 0) : (v5 = *i - 8),
            (v6 = *(_DWORD *)(v5 + 1348)) == 0
         || !(*(unsigned __int8 (**)(void))(*(_DWORD *)v6 + 4))()
         || (!*i ? (v7 = 0) : (v7 = *i - 8), (*(unsigned __int8 (**)(void))(*(_DWORD *)v7 + 48))())) )
      {
        if ( *i )
          v8 = *i - 8;
        else
          v8 = 0;
        sub_57DDD0(a2, (int)&v11, v8, v10);
      }
    }
  }
  return *(_DWORD *)(a2 + 8) != *(_DWORD *)(a2 + 12);
}