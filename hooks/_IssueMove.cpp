//HOOK OnMotionTurnEventChange ROffset = 0x002F2670

#include <stdlib.h>
#include "../preprocessor/define.h"
#include "../preprocessor/macro.h"

__asm__
(
    ".equ by_pass_address,"QU(dec_IssueMove)"-0x006F2670 \n"
);

__asm__ volatile
(
    "call . + by_pass_address \n"
    ".align 128, 0x0 \n"
);
