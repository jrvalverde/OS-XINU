/* test.c - print out 'hello world' in two parts, one part in each of two
 *	    tasks.  This is to test your system to see if it is running
 *	    correctly.
 */

main()
{
    int sem, other();

    sem = screate(0);		/* semaphore so that 'hello' is printed first */

    tstart(other,1,sem);	/* start executing at procedure 'other'
				 * with 1 arguments
				 */

    printf("hello ");

    ssignal(sem);		/* let other print 'world' */
}

other(sem)
int sem;
{
    swait(sem);			/* wait for main to print out 'hello ' */

    printf("world.\n");
}
