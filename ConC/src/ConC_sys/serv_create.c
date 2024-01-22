/* serv_create -- serv_create, serv_ioint, serv_illint */

#include <kernel.h>
#include <syscall.h>
#include <signal.h>

#undef	NULL

#include <stdio.h>

/*------------------------------------------------------------------------
 *  serv_create  --  create io_server
 *------------------------------------------------------------------------
 */

serv_create()
{
    int		fd, i, serv_ioint(), serv_illint(), save_errno;
    float	quant;
    char	sb[50];
    long	ps;

    save_errno = errno;

    if (!(server_pid = sys_fork())) {
	serv_enable();			/* disable all but SIGIO, SIGILL,
					 * SIGTSTP, SIGCONT interrupts
					 */

	sys_signal(SIGIO,serv_ioint);

	sys_signal(SIGILL,serv_illint);

	SYS_PIPE = Serv_SYS_PIPE;	/* set up server fd */

	io_server();
	}

    if ((fd = sys_open(TIME_QUANT_FILE,0,0)) >= 0) {/* optional time quant*/
	sys_read(fd,sb,sizeof(sb));
	sscanf(sb,"%f",&quant);
	squantum = (int) quant;
	usquantum = (int) ((quant - (float) squantum)*1000000.0);
	sys_close(fd);
	}

    for ( i = STDERR + 1 ; i < FD_SETSIZE ; i++ ) /* fclose fd's so we won't
	no_flush_fclose(i);			   * run out if user does a
						   * bunch of fclose's and
						   * fopen's, BUT DON'T
						   * FLUSH STREAMS (io_server
						   * will eventually do this)
						   */

    clock_interinit();			/* initialize clock interrupts	*/

    is_serv_init = 1;

    errno = save_errno;

    return(OK);
}

/*-------------------------------------------------------------------------
 * serv_ioint - receive interrupt from ConC system saying to quit
 *-------------------------------------------------------------------------
 */
LOCAL
serv_ioint()
{
    sys__exit(1);
}

/*-------------------------------------------------------------------------
 * serv_illint - receive illegal instruction interrupt from ConC system
 *		 possibly indicating more stack space should be allocated.
 *-------------------------------------------------------------------------
 */
LOCAL
serv_illint()
{
    kprintf("Illegal instruction trapped.  This may be caused by not\n");
    kprintf("allocating a large enough stack.  Use resume(create( ... ))\n");
    kprintf("and give a larger stack size.  If 'tstart' was used,\n");
    kprintf("give a larger stack size than the default of %d.\n",INITSTK);

    sys__exit(1);
}
