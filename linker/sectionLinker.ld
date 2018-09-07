/* Entry point; at the very start of the binary */
/*ENTRY(_ext_lua_LoadSavedGameHook)*/

INCLUDE Env.ld

MEMORY 
{
	rom (rwx)  : ORIGIN = 0x128D000, LENGTH = 0x500000
}

/* Binary sections 0x01672161 */
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

	.rdata BLOCK(align_size):
	{
		KEEP(*(.rdata))
	} > rom

	/* Read-write data (initialized) */
/* 	.data BLOCK(align_size) :
	{`
		*(.data)
	} Haven't made sector for this, so no globals, for now */

	/* Read-write data (uninitialized) and stack */
	/*.bss BLOCK(align_size) :
	{
		*(COMMON) 
		*(.bss)
	} nor this */
}