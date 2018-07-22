/* Entry point; at the very start of the binary */
/*ENTRY(_ext_lua_LoadSavedGameHook)*/

align_size = 0x100; /* squeeeeeze */

/* Binary sections */
SECTIONS
{
	/* '.' is the location counter. It counts locations. */
	. = 0x128B000;

	/* Code */
	.text BLOCK(align_size):
	{
		*(.ext)
		*(.text)
	}

	/* Read-only data. */
	.rdata BLOCK(align_size):
	{
		*(.rdata)
	}

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