/* creat.c - creat */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>

creat(name, mode)
char *name;
int  mode;
{
    int		   fd;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	fd = sys_creat(name,mode);

	if (fd >= 0) {
	    fd_tab[fd].pfd_head = FD_NULL;
	    fd_tab[fd].pfd_tail = FD_NULL;
	    }

	return(fd);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_creat;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= strlen(name)+1;
    sys_hdr.sys_val_len[1] 	= mode;

    proctab[currpid].sys_command = SYS_creat;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,name,strlen(name)+1);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
