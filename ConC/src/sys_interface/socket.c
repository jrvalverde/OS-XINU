/* socket.c - socket */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

socket(af, type, protocol)
int af, type, protocol;
{
    int fd;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	fd = sys_socket(af, type, protocol);

	if (fd >= 0)
	    fd_tab[fd].pfd_head = fd_tab[fd].pfd_tail = FD_NULL;

	return(fd);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_socket;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= af;
    sys_hdr.sys_val_len[1] 	= type;
    sys_hdr.sys_val_len[2] 	= protocol;

    proctab[currpid].sys_command = SYS_socket;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
