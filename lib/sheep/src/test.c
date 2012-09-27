#include <sheep.h>
#include <stdio.h>

int main()
{
	char str[512];
	
	while (1) {
		fgets(str, 512, stdin);
		sheep_exec_line(str, stderr);
	}
	
	return 0;
}
