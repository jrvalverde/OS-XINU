/* recvmsg.c - recvmsg */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

recvmsg(s, msg, flags)
int s;
struct msghdr msg[];
int flags;
{
    int i;
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_recvmsg(check_fd(s), msg, flags));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_recvmsg;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= flags;
    sys_hdr.sys_val_len[2] 	= msg -> msg_iovlen;
    sys_hdr.sys_val_len[3] 	= msg -> msg_accrightslen;

    proctab[currpid].sys_command = SYS_recvmsg;
    proctab[currpid].psys_args[0] = (long) msg;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	for ( i = 0 ; i < sys_hdr.sys_val_len[2] ; i++ )
	    sys_write(SYS_PIPE,&msg -> msg_iov[i].iov_len,sizeof(int));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
