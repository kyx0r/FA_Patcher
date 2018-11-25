//HOOK OnMotionTurnEventChange2 ROffset = 0x2B953F

//org 0x006B953F

#include <stdlib.h>

__asm__
(
    ".equ by_pass_address,0x006B9567-0x006B953F \n"
);

__asm__ __volatile__
(
    "jmp . + by_pass_address \n"
    ".align 128, 0x0 \n"
);




