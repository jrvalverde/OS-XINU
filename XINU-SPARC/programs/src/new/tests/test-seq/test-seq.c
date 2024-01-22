/* test-seq.c */

#include <sys/xinusys.h>
#include <vmem.h>

#define MKSMCLIENT 1
#define FREEHEAP

/****************************************************************************/
/************************* test routine ************************************/
/****************************************************************************/



/*---------------------------------------------------------------------------
 * test-seq - This is a simple test which writes into the heap area
 *---------------------------------------------------------------------------
 */
main(argc, argv)
int argc;
char *argv[];
{
	int	i, j, k;
	int	*a,*b;
	int 	nframes;
	long	stime;			/* start time */
	long	ttime;			/* total time */
	int	repeatcount;
	
	printf("test-seq: starting\n");
	printf("&i = 0x%08x, &j = 0x%08x\n", &i, &j);

	/* set nframes and repeatcount */
	nframes = (2*160) + 80; /* 160 pages = 1meg */
	repeatcount = 1;
#ifdef MKSMCLIENT
	nframes = 110;
	repeatcount = 4;
#endif
	for (i=1; i < argc; i++) {
		if (strcmp(argv[i], "-nf") == 0) {
			nframes = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-r") == 0) {
			repeatcount = atoi(argv[i+1]);
		}
	}
	printf("test-seq: setting nframes = %d, repeatcount = %d\n",
	       nframes, repeatcount);
	
	a = b = (int *)xgetheap((nframes+1)*PGSIZ);
	printf("test-seq: heap address = 0x%08x\n", (unsigned) b);
	
	if (a == (int *) SYSERR) {
		printf("test-seq: getheap returned SYSERR\n");
	}
	else {
 		if (xgetrealtim(&stime) == SYSERR) {
			printf("test-seq: ERROR: getutim failed\n");
			return;
		}
		for (k=0;k<repeatcount;k++) {
			printf("\nCurrent pass = %d\n", k);
			printf("now writing to pages\n");
			a = b;
			for (i=0; i<nframes; i++) {
				*a = i;
				if ((i!=0) && ((i%20)==0)) {
					printf("test-seq: i = %d, a = 0x%08x, *a = %d\n",
						i, (unsigned)a, *a);
				}
				a = (int *)(((unsigned)a) + PGSIZ);
			}
			
			printf("now reading pages back in\n");
			a = b;
			for (i=0; i<nframes; i++) {
				j =1;
				j = *a;
				if ((i!=0) && ((i%20)==0)) {
					printf("test-seq:i=%d, a=0x%08x, *a=%d, j=%d\n",
						i, (unsigned)a, *a, j);
				}
				a = (int *)(((unsigned)a) + PGSIZ);
			}
		}
		if (xgetrealtim(&ttime) == SYSERR) {
			printf("test-seq: ERROR: getutim failed\n");
			return;
		}
		ttime = ttime - stime;
		
		printf("test-seq: total run time = %ld ms\n", ttime*10);

#ifdef FREEHEAP
		if (xfreeheap(b, (nframes+1)*PGSIZ) == SYSERR) {
			printf("test-seq: freeheap returned SYSERR\n");
		}
#endif
	}
	printf("test-seq: HAS COMPLETED - now exiting\n");
}
	
