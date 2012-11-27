/*
* kernel.ld
*
* written by sjrct
*/

OUTPUT_FORMAT(binary)
ENTRY(kmain)

SECTIONS
{
	. = 0xFFFF80000000F000;
	
	.text :
	{
		* (.text)
		. = ALIGN(0x10);
	}

	.data :
	{
		* (.data)
		* (.rdata .rodata)
		. = ALIGN(0x10);
	}

	.bss :
	{
		* (.bss COMMON)
	}

	_bss_size = SIZEOF(.bss);
	_bss_addr = ADDR(.bss);

	/DISCARD/ :
	{
		* (.eh_frame)
		* (.comment)
	}
}
