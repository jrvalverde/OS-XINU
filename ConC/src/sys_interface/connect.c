/* connect.c - connect */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

connect(s, name, namelen)
int s;
struct sockaddr *name;
int namelen;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_connect(check_fd(s), name, namelen));

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_connect;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= namelen;
    sys_hdr.sys_val_len[2] 	= namelen;

    proctab[currpid].sys_command = SYS_connect;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,name,sys_hdr.sys_val_len[1]);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
