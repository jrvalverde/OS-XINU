/* dup2.c - dup2 */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

dup2(oldd,newd)
int oldd,newd;
{
    int		   ret_val;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	ret_val	= sys_dup2(check_fd(oldd),check_fd(newd));

	if (ret_val >= 0)
	    fd_tab[newd].pfd_head = fd_tab[newd].pfd_tail = FD_NULL;

	return(ret_val);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_dup2;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= oldd;
    sys_hdr.sys_val_len[1] 	= newd;

    proctab[currpid].sys_command = SYS_dup2;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
