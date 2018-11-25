//HOOK _aiinitattack prevents a crash when a target is in range but null is encountered. ROffset = 0x1F39F7

//org 0x005F39F7
#include <stdlib.h>

__asm__
(
    ".equ by_pass_address,0x00C0B4C2-0x005F39F7 \n"
);

__asm__ __volatile__
(
    "jmp . + by_pass_address \n"
    "nop \n"
    ".align 128, 0x0 \n"
);