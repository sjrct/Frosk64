//
// start.c
//
// written by sjrct
//

int main()
{
	unsigned long i;
	for (i = 0xB8000; i != 0xB8000 + 80 * 25 * 2; i += 2) {
		*((short*)i) = 0x5500;
	}
	while (1);
}
