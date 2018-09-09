#include "moho.h"
#include "funcDefs.h"
#include "global_func_table.h"
#include <stdlib.h>


int dword_10B83A4 = 0x010B83A4;

funcDefs Fd;
GFT gft; 

//-----	(006F26D0)	--------------------------------------------------------
int GFT::IssueMove(int *a1, double a2)
{
    int v2; //	esi
    int v3; //	eax
    uint32 * v4; //	eax
    bool v5; //	al
    bool v6; //	zf
    int v7; //	eax
    int v8; //	edi
    uint32 * v9; //	eax
    int * v10; //	esi
    int v11; //	eax
    int v12; //	xmm0_4
    int v13; //	eax
    int v14; //	eax
    int v15; //	esi
    int v17; //	[esp-8h]	[ebp-138h]
    int v18; //	[esp-4h]	[ebp-134h]
    bool v19; //	[esp+Fh]	[ebp-121h]
    int * v20; //	[esp+10h]	[ebp-120h]
    int v21; //	[esp+14h]	[ebp-11Ch]
    int * v22; //	[esp+18h]	[ebp-118h]
    int ** v23; //	[esp+1Ch]	[ebp-114h]
    char * v24; //	[esp+20h]	[ebp-110h]
    char * v25; //	[esp+24h]	[ebp-10Ch]
    int * v26; //	[esp+28h]	[ebp-108h]
    char * v27; //	[esp+2Ch]	[ebp-104h]
    char v28; //	[esp+30h]	[ebp-100h]
    int v29; //	[esp+40h]	[ebp-F0h]
    int v30; //	[esp+44h]	[ebp-ECh]
    int v31; //	[esp+48h]	[ebp-E8h]
    int v32; //	[esp+4Ch]	[ebp-E4h]
    int v33; //	[esp+50h]	[ebp-E0h]
    int v34; //	[esp+54h]	[ebp-DCh]
    int v35; //	[esp+58h]	[ebp-D8h]
    int v36; //	[esp+5Ch]	[ebp-D4h]
    int v37; //	[esp+60h]	[ebp-D0h]
    int v38; //	[esp+64h]	[ebp-CCh]
    int v39; //	[esp+68h]	[ebp-C8h]
    uint32 * v40; //	[esp+6Ch]	[ebp-C4h]
    int v41; //	[esp+70h]	[ebp-C0h]
    int v42; //	[esp+7Ch]	[ebp-B4h]
    char v43; //	[esp+90h]	[ebp-A0h]
    int v44; //	[esp+12Ch]	[ebp-4h]
	uint32 *v45;
	
    v2 = dword_10B83A4;
    v3 = Fd.lua_gettop(*a1);
    if (v3 != 2)
        Fd.LuaState__Error(a1, (char*)"%s\n	expected	%d	args,	but	got	%d", v2, 2, v3);
     v23 = & v22;
    v22 = (int * ) & v22;
    v24 = & v28;
    v25 = & v28;
    v26 = & v29;
    v27 = & v28;
    v44 = 0;
    v20 = a1;
    v21 = 1;
    v4 = Fd.Eval_Lua_Object( & v39, (int*)&v20, a1, (int)"IssueMove"); //unit table param from lua
    LOBYTE(v44) = 1;
    v5 = Fd.Validate_IssueCommand((int) v4, (int) &v22, 1);
    LOBYTE(v44) = 0;
    v19 = v5;
    if (v41 != v42)
        Fd.Goto_Exception_Handler_Ntdll(v41);
    v6 = v19 == 0;
	*v45 = (v39 + 4);
    v45 = v40;
    *v40 = v39;
    if (v6)
        goto LABEL_21;
    v20 = & v17;
    v7 = Fd.Get_Lua_Coordinates_State((int)&v39, a2, a1, (int)"IssueMove", a1, 2);
    Fd.Push_Coordinates((int)&v29, v7);
    v8 = v29;
    v36 = v31;
    v35 = v30;
    v9 = v40;
    v37 = v32;
    v34 = v29;
    v38 = v33;
    if (v40) 
	{
        if ((_DWORD ** ) * v40 != & v40) {
            do
                v9 = (_DWORD*)(*v9 + 4);
            while ((_DWORD **) * v9 != & v40);
        }
        *v9 = v41;
    }
     if (!(unsigned __int8) gft.nanTest((float*)& v36) || !v8)
        Fd.LuaState__Error(a1, "IssueMove:	Passed	in	an	invalid	target	point.");
     v10 = Moho_SSTICommandIssueData_SSTICommandIssueData((uint32)&v43,2);
    LOBYTE(v44) = 2;
    v11 = v30;
    v12 = v31;
    v10[4] = v8;
    v10[5] = v11;
    v13 = * a1;
    v10[6] = v12;
    v10[7] = v32;
    v18 = v13;
    v10[8] = v33;
    v14 = lua_getglobaluserdata(v18);
    v15 = Fd.Moho_UNIT_IssueCommand((int)&v22, v14, v10, 0);
    LOBYTE(v44) = 0;
    Fd.Moho_SSTICommandIssueData_Destructor_SSTICommandIssueData((int)&v43);
    if (!v15) 
	{
        LABEL_21: 
		Fd.LuaPlus_LuaState_PushNil(*a1);
        Fd.lua_gettop(*a1);
        if (v24 != v27)
            Fd.Goto_Exception_Handler_Ntdll(v24);
        v22[1] = (int) v23;
        * v23 = v22;
    }
    else {
        Fd.LuaPlus_LuaObject_PushStack((_DWORD * )(v15 + 32), v20, a1);
        gft.sub_5796A0(v22);
    }  
    return 1; 
}