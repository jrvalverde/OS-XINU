/* setsockopt.c - setsockopt */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

setsockopt(s, level, optname, optval, optlen)
int s, level, optname;
char *optval;
int optlen;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_setsockopt(check_fd(s), level, optname, optval, optlen));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_setsockopt;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= level;
    sys_hdr.sys_val_len[2] 	= optname;
    sys_hdr.sys_val_len[3] 	= optlen;

    proctab[currpid].sys_command = SYS_setsockopt;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,optval,sys_hdr.sys_val_len[3]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
