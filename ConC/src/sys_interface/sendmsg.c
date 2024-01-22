/* sendmsg.c - sendmsg */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

sendmsg(s, msg, flags)
int s;
struct msghdr msg[];
int flags;
{
    int i, sum_len;
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_sendmsg(check_fd(s), msg, flags));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_sendmsg;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= flags;
    sys_hdr.sys_val_len[2] 	= msg -> msg_namelen;
    sys_hdr.sys_val_len[3] 	= msg -> msg_iovlen;
    sys_hdr.sys_val_len[4] 	= msg -> msg_accrightslen;

    proctab[currpid].sys_command = SYS_sendmsg;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,msg -> msg_name,sys_hdr.sys_val_len[2]);
	for ( i = 0, sum_len = 0 ; i < sys_hdr.sys_val_len[3] ; i++ )
	    sum_len += msg -> msg_iov[i].iov_len;
	sys_write (SYS_PIPE,&sum_len,sizeof(int));
	sys_writev(SYS_PIPE,msg -> msg_iov,sys_hdr.sys_val_len[3]);
	sys_write(SYS_PIPE,msg -> msg_accrights,sys_hdr.sys_val_len[4]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
