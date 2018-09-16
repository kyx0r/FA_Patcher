//HOOK IssueMove ROffset = 0x002F266B

#include <stdlib.h>
#include "../preprocessor/define.h"
#include "../preprocessor/macro.h"

__asm__
(
	".equ by_pass_address,"QU(dec_IssueMove)"-0x006F266B \n"
);

__asm__ volatile 
(
	"call . + by_pass_address \n"
	"pop ebx \n"
	"ret \n"
	".align 128, 0x0 \n"
);
