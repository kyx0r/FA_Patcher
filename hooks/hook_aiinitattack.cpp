//HOOK _aiinitattack compare with a null. ROffset = 0x80B4C2

#include <stdlib.h>

//address on the left is the current instruction position.
//on the right, address is the final position. 
__asm__
(
	".equ by_pass_address,0x00C0B4C5-0x005F3B99 \n"
	".equ end_func_goto_thread_validation,0x00C0B4D1-0x005F39FD \n"
);

__asm__ __volatile__ 
(
	"cmp ecx,0 \n"
	"je . - by_pass_address \n"
	"mov edx,dword ptr [ecx] \n"
	"lea eax,dword ptr [ebp+0x60] \n"
	"push eax \n"
	"jmp . - end_func_goto_thread_validation \n"
	".align 128, 0x0 \n"
);