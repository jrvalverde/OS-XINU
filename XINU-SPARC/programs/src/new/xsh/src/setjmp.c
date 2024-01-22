
/* 
 * setjmp.c - 
 * 
 * Author:	
 * 		Computer Science Department
 * 		University of Kentucky
 * Date:	Fri Jun 26 09:08:55 1992
 */

/* This is a bogus file used to see whether setjmp is ever called */

#define DEBUG_LONGJMP_ONLY

setjmp()
{
#ifndef DEBUG_LONGJMP_ONLY
	printf("This version of setjmp does not work.\n");
	printf("It just lets you know that setjmp was called.\n");
	sleep(2);
	printf("Exiting\n");
	sleep(1);
	exit(1);
#endif
}

longjmp()
{
	printf("This version of longjmp does not work.\n");
	printf("It just lets you know that longjmp was called.\n");
	sleep(2);
	printf("Exiting\n");
	sleep(1);
	exit(1);
}

