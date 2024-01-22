/* findiop.c - findiniop() and declares _iob[] */

#include <stdio.h>
#define NUM_IOBUFS	32

#define inuse(iop)	((iop)->_file >= 0)

FILE _iob[NUM_IOBUFS] = {
	{ 0, NULL, (unsigned char *) "base string space",
	      0, _IOREAD|_IONBF, 0 },	/* stdin */
	{ 0, NULL, (unsigned char *) "base string space",
	      0, _IOWRT|_IONBF, 1 },	/* stdout */
	{ 0, NULL, (unsigned char *) "base string space",
	      0, _IOWRT|_IONBF, 2 },	/* stderr */
};

static char iob_initialized=0;


FILE *_findiop()
{
	int i;
	FILE *iop;
	FILE *fp;

	if (!iob_initialized) {
		for (i=3; i< (NUM_IOBUFS-3); i++)
		    _iob[i]._file = -1;
		iob_initialized = 1;
	}
	
	for(iop = _iob; iop < _iob + NUM_IOBUFS; iop++)
		if(!inuse(iop))
			return(iop);
	return(NULL);			/* none found */
}

