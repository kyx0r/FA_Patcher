//HOOK OnMotionTurnEventChange2 ROffset = 0x2BFF2C

//org 0x006BFF2C

#include <stdlib.h>

__asm__
(
    ".equ by_pass_address,0x006BFF55-0x006BFF2C \n"
);

__asm__ __volatile__
(
    "jmp . + by_pass_address \n"
    ".align 128, 0x0 \n"
);








