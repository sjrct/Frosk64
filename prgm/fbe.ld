/*
* kernel.ld
*
* written by sjrct
*/

OUTPUT_FORMAT(binary)
ENTRY(main)

SECTIONS
{
	. = 0x100000 - 0x10;
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

	_heap_offset = . - 0x100000;

	/DISCARD/ :
	{
		* (.eh_frame)
		* (.comment)
	}
}
