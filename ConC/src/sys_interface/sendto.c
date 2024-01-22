/* sendto.c - sendto */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

sendto(s, msg, len, flags, to, tolen)
int s;
char *msg;
int len, flags;
struct sockaddr *to;
int tolen;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_sendto(check_fd(s), msg, len, flags, to, tolen));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_sendto;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= len;
    sys_hdr.sys_val_len[2] 	= flags;
    sys_hdr.sys_val_len[3] 	= tolen;

    proctab[currpid].sys_command = SYS_sendto;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,msg,sys_hdr.sys_val_len[1]);
	sys_write(SYS_PIPE,to,sys_hdr.sys_val_len[3]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
