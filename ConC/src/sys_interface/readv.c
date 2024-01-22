/* readv.c - readv */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>

readv(d, iov, iovcnt)
int d;
struct iovec *iov;
int iovcnt;
{
    int i;
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_readv(check_fd(d), iov, iovcnt));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_readv;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;
    sys_hdr.sys_val_len[1] 	= iovcnt;

    proctab[currpid].sys_command = SYS_readv;
    proctab[currpid].psys_args[0] = (long) iov;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	for ( i = 0 ; i < iovcnt ; i++ )
	    sys_write(SYS_PIPE,(char *)&iov[i].iov_len,sizeof(int));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
