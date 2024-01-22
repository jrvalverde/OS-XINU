/* interrupt.c - sys_interinit, clock_interinit, clkvtint, clkrtint,
 *		 intint, illint, quitint, ioint, Flush_Pipe
 */

#include <kernel.h>
#include <proc.h>
#include <syscall.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

#define min(x,y)	(((x) < (y)) ? (x) : (y))

int	clkvtint();
int	clkrtint();
int	intint();
int	ioint();
int	illint();
int	quitint();
int	(*sys_signal())();

/*-------------------------------------------------------------------------
 * sys_interinit - initialize system interrupt service routines
 *-------------------------------------------------------------------------
 */
sys_interinit()
{
    rtival.it_value.tv_sec = rtival.it_interval.tv_sec = 0;
    rtival.it_value.tv_usec = rtival.it_interval.tv_usec = 0;
    sys_signal(SIGALRM,clkrtint);

    sys_signal(SIGINT,intint);

    sys_signal(SIGIO,ioint);

    sys_signal(SIGILL,illint);

    sys_signal(SIGQUIT,quitint);
}

/*-------------------------------------------------------------------------
 * clock_interinit - init virtual timer interrupt routine (for time slices)
 *-------------------------------------------------------------------------
 */
clock_interinit()
{
    vtival.it_value.tv_sec  = squantum;
    vtival.it_value.tv_usec = usquantum;
    vtival.it_interval.tv_sec = 0;
    vtival.it_interval.tv_usec = 0;

    sys_setitimer(ITIMER_VIRTUAL,&vtival,&vtioval);
    sys_signal(SIGVTALRM,clkvtint);
}

/*-------------------------------------------------------------------------
 * clkvtint - virtual time "time-slice" interrupt service routine
 *-------------------------------------------------------------------------
 */
LOCAL
clkvtint()
{
    long ps;
    disable(ps);

    sys_signal(SIGVTALRM,clkvtint);

    resched();

    restore(ps);
}

/*-------------------------------------------------------------------------
 * clkrtint - real time "sleep" interrupt service routine
 *-------------------------------------------------------------------------
 */
LOCAL
clkrtint()
{
    long ps;
    disable(ps);

    sys_signal(SIGALRM,clkrtint);

    wakeup();

    restore(ps);
}

/*-------------------------------------------------------------------------
 * intint - SIGINT interrupt service routine
 *-------------------------------------------------------------------------
 */
LOCAL
intint()
{
    sys_kill(server_pid,SIGIO);

    sys__exit(1);
}

/*-------------------------------------------------------------------------
 * quitint - SIGQUIT interrupt service routine
 *-------------------------------------------------------------------------
 */
LOCAL
quitint()
{
    sys_kill(server_pid,SIGIO);

    sys__exit(1);
}

/*-------------------------------------------------------------------------
 * illint - SIGILL interrupt service routine
 *-------------------------------------------------------------------------
 */
LOCAL
illint()
{
    sys_kill(server_pid,SIGILL);

    sys__exit(1);
}

/*-------------------------------------------------------------------------
 * ioint - SIGIO interrupt service routine.
 *	   Gets IO requests back from io_server.
 *-------------------------------------------------------------------------
 */
LOCAL
ioint()
{
    int nfound,nfds;
    fd_set readfds,writefds,exceptfds;
    long ps;
    struct sys_hdr sys_hdr;
    struct timeval timeout;
    struct pentry *pptr;

    disable(ps);

    timeout.tv_sec = timeout.tv_usec = 0;

    nfds = SYS_PIPE + 1;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    FD_SET(SYS_PIPE, &readfds);

    while(sys_select(nfds,&readfds,&writefds,&exceptfds,&timeout) > 0) {
	Read_From_Pipe(&sys_hdr,sizeof(sys_hdr));
	if (isbadpid(sys_hdr.sys_pid)
			|| (pptr = &proctab[sys_hdr.sys_pid])->pstate==PRFREE
			||  pptr -> sys_command != sys_hdr.sys_command) {
	    kprintf("Warning:\tsyscall header incorrect.\n");
	    Flush_Pipe(sys_hdr);
	    }
	else {
	    pptr -> sys_ret_val = sys_hdr.sys_ret_val;
	    pptr -> sys_errno	= sys_hdr.sys_errno;
	    if (sys_hdr.sys_pid == gettid())
		errno = sys_hdr.sys_errno;
	    switch (sys_hdr.sys_command) {
		case SYS_pipe:				/* args[0] = fildes   */
			if (sys_hdr.sys_ret_val >= 0) {
			    ((int *)(pptr->psys_args[0]))[0] =
							sys_hdr.sys_val_len[0];
			    ((int *)(pptr->psys_args[0]))[1] =
							sys_hdr.sys_val_len[1];
			    }
			break;

		case SYS_read:				/* args[0] = buf      */
			Read_From_Pipe(pptr->psys_args[0],sys_hdr.sys_ret_val);
			break;

		case SYS_fstat:				/* args[0] = buf      */
			if (sys_hdr.sys_ret_val >= 0)
			    Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_val_len[1]);
			break;

		case SYS_ioctl:				/* args[0] = argp     */
			if (sys_hdr.sys_ret_val >= 0)
			    Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_val_len[2]);
			break;

		case SYS_accept:			/* args[0] = addr     */
							/* args[1] = addrlen  */
			if (sys_hdr.sys_ret_val >= 0) {
			    *((int *)(pptr->psys_args[1])) =
							sys_hdr.sys_val_len[1];
			    Read_From_Pipe(pptr->psys_args[0],
							sys_hdr.sys_val_len[1]);
			    }
			break;

		case SYS_getpeername:			/* args[0] = name     */
							/* args[1] = namelen  */
			if (sys_hdr.sys_ret_val >= 0) {
			    *((int *)(pptr->psys_args[1])) =
							sys_hdr.sys_val_len[1];
			    Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_val_len[1]);
			    }
			break;

		case SYS_getsockname:			/* args[0] = name     */
							/* args[1] = namelen  */
			if (sys_hdr.sys_ret_val >= 0) {
			    *((int *)(pptr->psys_args[1])) =
							sys_hdr.sys_val_len[1];
			    Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_val_len[1]);
			    }
			break;

		case SYS_getsockopt:			/* args[0] = optval   */
							/* args[1] = optlen   */
			if (sys_hdr.sys_ret_val >= 0) {
			    *((int *)(pptr->psys_args[1])) =
							sys_hdr.sys_val_len[3];
			    Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_val_len[3]);
			    }
			break;

		case SYS_readv:				/* args[0] = iov      */
			if (sys_hdr.sys_ret_val > 0) {
			    int i, cc_togo = sys_hdr.sys_ret_val;

			    for ( i = 0 ; cc_togo > 0 ; i++ )
				cc_togo -= Read_From_Pipe(
					((struct iovec *)
					(pptr -> psys_args[0]))[i].iov_base,
				    min(cc_togo,((struct iovec *)
					(pptr -> psys_args[0]))[i].iov_len));
			    }
			break;

		case SYS_recv:				/* args[0] = buf      */
			Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_ret_val);
			break;

		case SYS_recvfrom:			/* args[0] = buf      */
							/* args[1] = from     */
							/* args[2] = fromlen  */
			if (sys_hdr.sys_ret_val > 0) {
			    Read_From_Pipe(pptr -> psys_args[0],
							sys_hdr.sys_ret_val);
			    *((int *)(pptr->psys_args[2])) =
							sys_hdr.sys_val_len[3];
			    Read_From_Pipe(pptr -> psys_args[1],
							sys_hdr.sys_val_len[3]);
			    }
			break;

		case SYS_recvmsg:			/* args[0] = msg      */
			if (sys_hdr.sys_ret_val > 0) {
			    int i, cc_togo = sys_hdr.sys_ret_val;

			    for ( i = 0 ; cc_togo > 0 ; i++ )
				cc_togo -= Read_From_Pipe(
				  ((struct msghdr *)
				    (pptr->psys_args[0]))->msg_iov[i].iov_base,
				  min(cc_togo,((struct msghdr *)
				    (pptr->psys_args[0]))->msg_iov[i].iov_len));

			    Read_From_Pipe(
				   ((struct msghdr *)
				     (pptr->psys_args[0]))->msg_accrights,
				   ((struct msghdr *)
				     (pptr->psys_args[0]))->msg_accrightslen);
			    }
			break;

		case SYS_socketpair:			/* args[0] = sv       */
			if (sys_hdr.sys_ret_val >= 0) {
			    ((int *)(pptr->psys_args[0]))[0] =
							sys_hdr.sys_val_len[3];
			    ((int *)(pptr->psys_args[0]))[1] =
							sys_hdr.sys_val_len[4];
			    }
			break;
		}
	    ssignal(proctab[sys_hdr.sys_pid].sys_sem);	/* signal personal sem*/
	    }

	timeout.tv_sec = timeout.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(SYS_PIPE, &readfds);
	}

    sys_signal(SIGIO,ioint);

    restore(ps);
}

/*-------------------------------------------------------------------------
 * Flush-Pipe - Flush the pipe on a bad sys_hdr.
 *-------------------------------------------------------------------------
 */
LOCAL
Flush_Pipe(sys_hdr)
struct sys_hdr sys_hdr;
{
    char *dummy;

    switch (sys_hdr.sys_command) {
	case SYS_read:				/* args[0] = buf      */
		dummy = MALLOC(char *,sys_hdr.sys_ret_val);
		Read_From_Pipe(dummy,sys_hdr.sys_ret_val);
		FREE(char *,dummy);
		break;

	case SYS_fstat:				/* args[0] = buf      */
		if (sys_hdr.sys_ret_val >= 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_val_len[1]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[1]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_ioctl:				/* args[0] = argp     */
		if (sys_hdr.sys_ret_val >= 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_val_len[2]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[2]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_accept:			/* args[0] = addr     */
						/* args[1] = addrlen  */
		if (sys_hdr.sys_ret_val >= 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_val_len[1]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[1]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_getpeername:			/* args[0] = name     */
						/* args[1] = namelen  */
		if (sys_hdr.sys_ret_val >= 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_val_len[1]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[1]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_getsockname:			/* args[0] = name     */
						/* args[1] = namelen  */
		if (sys_hdr.sys_ret_val >= 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_val_len[1]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[1]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_getsockopt:			/* args[0] = optval   */
						/* args[1] = optlen   */
		if (sys_hdr.sys_ret_val >= 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_val_len[3]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[3]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_readv:				/* args[0] = iov      */
		dummy = MALLOC(char *,sys_hdr.sys_ret_val);
		Read_From_Pipe(dummy,sys_hdr.sys_ret_val);
		FREE(char *,dummy);

		break;

	case SYS_recv:				/* args[0] = buf      */
		dummy = MALLOC(char *,sys_hdr.sys_ret_val);
		Read_From_Pipe(dummy,sys_hdr.sys_ret_val);
		FREE(char *,dummy);

		break;

	case SYS_recvfrom:			/* args[0] = buf      */
						/* args[1] = from     */
						/* args[2] = fromlen  */
		if (sys_hdr.sys_ret_val > 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_ret_val);
		    Read_From_Pipe(dummy,sys_hdr.sys_ret_val);
		    FREE(char *,dummy);

		    dummy = MALLOC(char *,sys_hdr.sys_val_len[3]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[3]);
		    FREE(char *,dummy);
		    }
		break;

	case SYS_recvmsg:			/* args[0] = msg      */
		if (sys_hdr.sys_ret_val > 0) {
		    dummy = MALLOC(char *,sys_hdr.sys_ret_val);
		    Read_From_Pipe(dummy,sys_hdr.sys_ret_val);
		    FREE(char *,dummy);

		    dummy = MALLOC(char *,sys_hdr.sys_val_len[3]);
		    Read_From_Pipe(dummy,sys_hdr.sys_val_len[3]);
		    FREE(char *,dummy);
		    }
		break;
	}
}

int
Read_From_Pipe(sb,len)
char *sb;
int len;
{
    int cc_togo;

    for (cc_togo = len ; cc_togo > 0 ; )
	cc_togo -= sys_read(SYS_PIPE, &sb[len - cc_togo], cc_togo);

    return(len);
}
