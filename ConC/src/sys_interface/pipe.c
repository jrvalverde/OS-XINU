/* pipe.c - pipe */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>

pipe(fildes)
int fildes[2];
{
    int		   ret_val;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	ret_val	= sys_pipe(fildes);

	if (ret_val >= 0) {
	    fd_tab[fildes[0]].pfd_head	= fd_tab[fildes[0]].pfd_tail = FD_NULL;
	    fd_tab[fildes[1]].pfd_head	= fd_tab[fildes[1]].pfd_tail = FD_NULL;
	    }

	return(ret_val);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_pipe;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;

    proctab[currpid].sys_command = SYS_pipe;
    proctab[currpid].psys_args[0] = (long)fildes;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
