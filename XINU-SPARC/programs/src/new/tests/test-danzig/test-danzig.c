/* test-danzig.c */

#include <sys/xinusys.h>

/****************************************************************************/
/************************* test routine ************************************/
/****************************************************************************/



/*---------------------------------------------------------------------------
 * test-danzig - Test the Danzig/Melvin Timer Board Delay from user space
 *---------------------------------------------------------------------------
 */
main(argc, argv)
int argc;
char *argv[];
{
	long	tim1,tim2;

	/* pray these get executed without a context switch */
	xgetrealtim(&tim1);
	xgetrealtim(&tim2);

	/* print the delay */
	if ((tim2-tim1) == 0) {
		printf("\nDanzig/Melvin time board is not installed\n\n");
		return;
	}
	printf("\nDanzig/Melvin time board is installed\n");
	printf("The delay to read the chip ");
	printf("from user space is %ld microseconds\n\n", tim2-tim1);
	
}	
