//HOOK OnMotionTurnEventChange ROffset = 0x2B8FB9

//org 0x006B8FB9
#include <stdlib.h>

//int by_pass_address asm("address") = 0x006B8FE0;

__asm__
(
    ".equ by_pass_address,0x006B8FE0-0x006B8FB9 \n"
);

__asm__ __volatile__
(
    "jmp . + by_pass_address \n"
    ".align 128, 0x0 \n"
);