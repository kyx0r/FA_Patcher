//HOOK OnMotionTurnEventChange ROffset = 0x2B8FB9

//org 0x006B8FB9
#include <stdlib.h>

//int by_pass_address asm("address") = 0x006B8FE0;

//int by_pass_address = 0x006B8FE0;

__asm__ volatile 
(
	".byte 0xE9 \n"
	".byte 0x22 \n"
	".byte 0x0 \n"
	".byte 0x0 \n"
	".byte 0x0 \n"
	".byte 0x0 \n"
	".byte 0x0 \n"
	".byte 0x0 \n"
);

/* int src = 1;
int dst;   

asm ("mov %1, %0\n\t"
    "add $1, %0"
    : "=r" (dst) 
    : "r" (src)); */