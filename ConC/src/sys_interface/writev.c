/* writev.c - writev */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>

writev(d, iov, ioveclen)
int d;
struct iovec *iov;
int ioveclen;
{
    int i, sum_len;
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_writev(check_fd(d), iov, ioveclen));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_writev;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;
    sys_hdr.sys_val_len[1] 	= ioveclen;

    proctab[currpid].sys_command = SYS_writev;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	for ( i = 0, sum_len = 0 ; i < sys_hdr.sys_val_len[1] ; i++ )
	    sum_len += iov[i].iov_len;
	sys_write (SYS_PIPE,&sum_len,sizeof(int));
	sys_writev(SYS_PIPE,iov,sys_hdr.sys_val_len[1]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
