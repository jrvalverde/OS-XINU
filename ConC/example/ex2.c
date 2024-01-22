/* ex2.c - print out 'hello world' after creating io_server
 *	   (io_server is started when a task other than main
 *	   is created or started).
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
