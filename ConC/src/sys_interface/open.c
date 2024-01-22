/* open.c - open */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>

open(path,flags,mode)
char *path;
int flags, mode;
{
    int		   fd;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	fd = sys_open(path,flags,mode);

	if (fd >= 0)
	    fd_tab[fd].pfd_head = fd_tab[fd].pfd_tail = FD_NULL;

	return(fd);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_open;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= strlen(path)+1;
    sys_hdr.sys_val_len[1] 	= flags;
    sys_hdr.sys_val_len[2] 	= mode;

    proctab[currpid].sys_command = SYS_open;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,path,strlen(path)+1);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
