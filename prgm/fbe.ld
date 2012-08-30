/*
* kernel.ld
*
* written by sjrct
*/

OUTPUT_FORMAT(binary)
ENTRY(main)

SECTIONS
{
	. = 0x100000;
	.fbehdr :
	{
		LONG(0xFBE0FBE0)
		LONG(SIZEOF(.text))
		LONG(SIZEOF(.data))
		LONG(SIZEOF(.bss))
	}

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
