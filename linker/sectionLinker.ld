/* Entry point; at the very start of the binary */
/*ENTRY(_ext_lua_LoadSavedGameHook)*/

MEMORY 
{
	rom (rwx)  : ORIGIN = 0x128D000, LENGTH = 0x500000
}


align_size = 0x1000; /* squeeeeeze */

/* Binary sections */
SECTIONS
{
	/* '.' is the location counter. It counts locations. */
	. = 0x128D000;

	/* Code */
	.text BLOCK(align_size):
	{
		*(.exxt)
		*(.text)
	} > rom

	.rdata 0x01672161:
	{
		KEEP(*(.rdata))
	} > rom

	/* Read-write data (initialized) */
	/*.data BLOCK(align_size) :
	{`
		*(.data)
	} Haven't made sector for this, so no globals, for now*/

	/* Read-write data (uninitialized) and stack */
	/*.bss BLOCK(align_size) :
	{
		*(COMMON) 
		*(.bss)
	} nor this */
}