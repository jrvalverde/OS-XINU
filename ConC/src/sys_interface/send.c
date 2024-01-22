/* send.c - send */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

send(s, msg, len, flags)
int s;
char *msg;
int len, flags;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_send(check_fd(s), msg, len, flags));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_send;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= len;
    sys_hdr.sys_val_len[2] 	= flags;

    proctab[currpid].sys_command = SYS_send;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,msg,sys_hdr.sys_val_len[1]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
