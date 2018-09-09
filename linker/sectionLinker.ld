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
	.text BLOCK(align_data):
	{
		*(.exxt)
		*(.text)
	} > rom

	.data BLOCK(align_data):
	{
		*(.data)
	} > rom
	
	.rdata BLOCK(align_rdata):
	{
		KEEP(*(.rdata))
	} > rom

	.bss BLOCK(align_bss):
	{
		*(.bss)
	} > rom
	
	.idata BLOCK(align_idata):
	{
		*(.idata)
	} > rom
}