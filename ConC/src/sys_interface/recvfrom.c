/* recvfrom.c - recvfrom */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

recvfrom(s, buf, len, flags, from, fromlen)
int s;
char *buf;
int len, flags;
struct sockaddr *from;
int *fromlen;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_recvfrom(check_fd(s), buf, len, flags, from, fromlen));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_recvfrom;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= len;
    sys_hdr.sys_val_len[2] 	= flags;
    sys_hdr.sys_val_len[3] 	= *fromlen;

    proctab[currpid].sys_command = SYS_recvfrom;
    proctab[currpid].psys_args[0] = (long) buf;
    proctab[currpid].psys_args[1] = (long) from;
    proctab[currpid].psys_args[2] = (long) fromlen;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
