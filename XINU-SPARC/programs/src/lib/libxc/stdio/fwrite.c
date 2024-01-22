/* fwrite.c - fwrite() */

#include <stdio.h>

#ifdef MIPS
size_t
#endif
fwrite(ptr, size, count, ioptr)
char *ptr;
int size, count;
FILE *ioptr;
{
	unsigned togo;

	if (size <= 0 || count <= 0)
	        return (0);

	togo = count*size;
	while (togo > 512) {
		write(fileno(ioptr), ptr, 512);
		ptr += 512;
		togo -= 512;
	}

	write(fileno(ioptr), ptr, togo);
	return(count);
	
}
