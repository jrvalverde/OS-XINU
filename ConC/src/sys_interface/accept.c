/* accept.c - accept */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

accept(s, addr, addrlen)
int s;
struct sockaddr *addr;
int *addrlen;
{
    int fd;
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	fd = sys_accept(check_fd(s), addr, addrlen);

	if (fd >= 0) 
	    fd_tab[fd].pfd_head = fd_tab[fd].pfd_tail = FD_NULL;

	return(fd);
	}

    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_accept;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= s;
    sys_hdr.sys_val_len[1] 	= *addrlen;
    sys_hdr.sys_val_len[2] 	= *addrlen;

    proctab[currpid].sys_command = SYS_accept;
    proctab[currpid].psys_args[0] = (long) addr;
    proctab[currpid].psys_args[1] = (long) addrlen;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
