#include "include/funcDefs.h"
#include "include/global_func_table.h"
#include <stdlib.h>


int dword_10B83A4 = 0x010B83A4;

funcDefs Fd;
GFT gft; 

//-----	(006F26D0)	--------------------------------------------------------
int GFT::IssueMove()
{	
	register int eax asm("eax");
	register int esp asm("esp");
	register int ecx asm("ecx");
	register int edx asm("edx");
	register int ebx asm("ebx");
	register int esi asm("esi");
	register int edi asm("edi");
__asm__
( 
	"pop ebx \n "
	"pop esi \n " 	
	"pop edi \n " 
	"push ebp \n " 
	"mov ebp,esp \n " 
	"and esp,0xFFFFFFF8 \n " 
	"mov eax,dword ptr [fs:0x0] \n " 
	"push 0xFFFFFFFF \n " 
	"push 0xBB5873 \n " 
	"push eax \n " 
	"mov dword ptr [fs:0x0],esp \n " 
	"mov eax,dword ptr [ebx] \n " 
	"sub esp,0x11C \n " 
	"push esi \n " 
	"mov esi,dword ptr [0x10B83A4] \n " 
	"push edi \n " 
);
	Fd.lua_gettop(eax);
__asm__ (
	"add esp,0x4 \n " 
	"cmp eax,0x2 \n " 
	"je label1 \n " 
);
	Fd.LuaState__Error(ebx, (char*)"%s\n	expected	%d	args,	but	got	%d", esi, 2, eax);
__asm__ (
	"add esp,0x14 \n " 
	"label1: \n"
	"lea ecx,dword ptr [esp+0x18] \n " 
	"mov edx,ecx \n " 
	"lea eax,dword ptr [esp+0x30] \n " 
	"mov dword ptr [esp+0x1C],ecx \n " 
	"mov dword ptr [esp+0x18],edx \n " 
	"mov ecx,eax \n " 
	"lea edx,dword ptr [esp+0x40] \n " 
	"mov dword ptr [esp+0x20],eax \n " 
	"mov dword ptr [esp+0x24],ecx \n " 
	"mov dword ptr [esp+0x28],edx \n " 
	"mov dword ptr [esp+0x2C],eax \n " 
	"push 0xE2ED00 \n " 
	"push ebx \n " 
	"lea ecx,dword ptr [esp+0x18] \n " 
	"push ecx \n " 
	"lea edx,dword ptr [esp+0x74] \n " 
	"mov dword ptr [esp+0x138],0 \n " 
	"push edx \n " 
	"mov dword ptr [esp+0x20],ebx \n " 
	"mov dword ptr [esp+0x24],1 \n " 
	"call 7274048 \n " 
);
__asm__ (
	"lea ecx,dword ptr [esp+0x28] \n " 
	"push 0x1 \n " 
	"push ecx \n " 
	"mov edi,eax \n " 
	"mov byte ptr [esp+0x144],1 \n " 
	"call 7273712 \n " 
);
	//Fd.Validate_IssueCommand(edi, ecx, 1);
__asm__ (
	"mov byte ptr [esp+0x144],0 \n " 
	"mov byte ptr [esp+0x27],al \n " 
	"mov eax,dword ptr [esp+0x88] \n " 
	"add esp,0x18 \n " 
	"cmp eax,dword ptr [esp+0x7C] \n " 
	"je label3 \n " 
);
	Fd.j_shi_delete_0(eax);
__asm__ (
	"add esp,0x4 \n " 
	"label3: \n"
	"cmp byte ptr [esp+0xF],0 \n " 
	"mov edx,dword ptr [esp+0x68] \n " 
	"mov eax,dword ptr [esp+0x6C] \n " 
	"mov dword ptr [edx+0x4],eax \n " 
	"mov ecx,dword ptr [esp+0x6C] \n " 
	"mov edx,dword ptr [esp+0x68] \n " 
	"mov dword ptr [ecx],edx \n " 
	"je label2 \n " 
	"sub esp,0x8 \n " 
	"mov eax,esp \n " 
	"mov dword ptr [esp+0x18],esp \n " 
	"push 0xE2ED00 \n " 
	"push ebx \n " 
	"lea edi,dword ptr [esp+0x78] \n " 
	"mov dword ptr [eax],ebx \n " 
	"mov dword ptr [eax+0x4],2 \n " 
);
	//Fd.Get_Lua_Coordinates_State(,(int)"IssueMove",ebx,2);
__asm__ (
	"call 7274336 \n " 
	"mov ecx,eax \n " 
	"add esp,0x10 \n " 
	"lea eax,dword ptr [esp+0x40] \n " 
);
__asm__ (
	"call 6174672 \n " 
	"mov eax,dword ptr [esp+0x44] \n " 
	"movss xmm0,dword ptr [esp+0x48] \n " 
	"mov edi,dword ptr [esp+0x40] \n " 
	"movss dword ptr [esp+0x5C],xmm0 \n " 
	"movss xmm0,dword ptr [esp+0x4C] \n " 
	"mov dword ptr [esp+0x58],eax \n " 
	"mov eax,dword ptr [esp+0x6C] \n " 
	"test eax,eax \n " 
	"movss dword ptr [esp+0x60],xmm0 \n " 
	"movss xmm0,dword ptr [esp+0x50] \n " 
	"mov dword ptr [esp+0x54],edi \n " 
	"movss dword ptr [esp+0x64],xmm0 \n " 
	"je label4 \n " 
	"lea ecx,dword ptr [esp+0x6C] \n " 
	"cmp dword ptr [eax],ecx \n " 
	"je label5 \n " 
	"lea ecx,dword ptr [ecx] \n " 
	"label6:"
	"mov eax,dword ptr [eax] \n " 
	"add eax,0x4 \n " 
	"lea edx,dword ptr [esp+0x6C] \n " 
	"cmp dword ptr [eax],edx \n " 
	"jne label6 \n " 
	"label5:"	
	"mov ecx,dword ptr [esp+0x70] \n " 
	"mov dword ptr [eax],ecx \n " 
	"label4:"
	"lea esi,dword ptr [esp+0x5C] \n " 
);
__asm__ (
	"call 5662704 \n " 
	"test al,al \n " 
	"je label7 \n " 
	"test edi,edi \n " 
	"jne label8 \n " 
	"label7:"
	"push 0xE2ED0C \n " 
	"push ebx \n " 
);
__asm__ (
	"call 9490896 \n " 
	"add esp,0x8 \n " 
	"label8:"
	"lea edx,dword ptr [esp+0x90] \n " 
	"push edx \n " 
	"mov ecx,0x2 \n " 
);
__asm__ (
	"call 5584208 \n " 
	"mov esi,eax \n " 
	"mov byte ptr [esp+0x12C],2 \n " 
	"mov eax,dword ptr [esp+0x44] \n " 
	"movss xmm0,dword ptr [esp+0x48] \n " 
	"mov dword ptr [esi+0x10],edi \n " 
	"mov dword ptr [esi+0x14],eax \n " 
	"mov eax,dword ptr [ebx] \n " 
	"movss dword ptr [esi+0x18],xmm0 \n " 
	"movss xmm0,dword ptr [esp+0x4C] \n " 
	"movss dword ptr [esi+0x1C],xmm0 \n " 
	"movss xmm0,dword ptr [esp+0x50] \n " 
	"push eax \n " 
	"movss dword ptr [esi+0x20],xmm0 \n " 
);
__asm__ (
	"call 9588816 \n " 
	"add esp,0x4 \n " 
	"push 0x0 \n " 
	"push esi \n " 
	"push eax \n " 
	"lea edx,dword ptr [esp+0x24] \n " 
);
__asm__ (
	"call 7283392 \n " 
	"add esp,0xC \n " 
	"lea ecx,dword ptr [esp+0x90] \n " 
	"push ecx \n " 
	"mov esi,eax \n " 
	"mov byte ptr [esp+0x130],0 \n " 
);
__asm__ (
	"call 5749680 \n " 
	"test esi,esi \n " 
	"je label2 \n " 
	"push ebx \n " 
	"lea edx,dword ptr [esp+0x14] \n " 
	"push edx \n " 
	"lea ecx,dword ptr [esi+0x20] \n " 
);
__asm__ (
	"call 9473408 \n " 
	"lea esi,dword ptr [esp+0x18] \n " 
);
__asm__ (
	"call 5744288 \n " 
	"jmp label9 \n " 
	"label2:"
	"mov eax,dword ptr [ebx] \n " 
	"push eax \n " 
);
__asm__ (
	"call 9493760 \n " 
	"mov ecx,dword ptr [ebx] \n " 
	"add esp,0x4 \n " 
	"push ecx \n " 
);
__asm__ (
	"call 9491856 \n " 
	"mov eax,dword ptr [esp+0x24] \n " 
	"add esp,0x4 \n " 
	"cmp eax,dword ptr [esp+0x2C] \n " 
	"je label10 \n " 
	"push eax \n " 
);
__asm__ (
	"call 11023682 \n " 
	"add esp,0x4 \n " 
	"label10:"
	"mov edx,dword ptr [esp+0x18] \n " 
	"mov eax,dword ptr [esp+0x1C] \n " 
	"mov dword ptr [edx+0x4],eax \n " 
	"mov ecx,dword ptr [esp+0x1C] \n " 
	"mov edx,dword ptr [esp+0x18] \n " 
	"mov dword ptr [ecx],edx \n " 
	"label9:"
	);
__asm__ (	
	"mov ecx,dword ptr [esp+0x124] \n " 
	"pop edi \n " 
	"mov eax,0x1 \n " 
	"mov dword ptr [fs:0x0],ecx \n " 
	"pop esi \n " 
	"mov esp,ebp \n " 
	"pop ebp \n " 
	"ret \n " 
	);

}