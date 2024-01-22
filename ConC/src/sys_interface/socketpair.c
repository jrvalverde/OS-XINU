/* socketpair.c - socketpair */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

socketpair(d, type, protocol, sv)
int d, type, protocol;
int sv[2];
{
    int		   ret_val;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	ret_val = sys_socketpair(d, type, protocol, sv);

	if (ret_val >= 0) {
	    fd_tab[sv[0]].pfd_head = fd_tab[sv[0]].pfd_tail = FD_NULL;
	    fd_tab[sv[1]].pfd_head = fd_tab[sv[1]].pfd_tail = FD_NULL;
	    }

	return(ret_val);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_socketpair;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0]	= d;
    sys_hdr.sys_val_len[1]	= type;
    sys_hdr.sys_val_len[2]	= protocol;

    proctab[currpid].sys_command = SYS_socketpair;
    proctab[currpid].psys_args[0] = (long)sv;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
