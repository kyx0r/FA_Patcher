
INCLUDE Env.ld

/*MEMORY 
{
	rom (rx)  : ORIGIN = 0x006B8FB9, LENGTH = 0x1000
}*/

/* Binary sections */
SECTIONS
{
	. = 0x00000000;
	
	.text BLOCK(align_size):
	{
		*(.ext)
		*(.text)
	}
	
	.rdata BLOCK(align_size):
	{
		*(.rdata)
	}
	
}