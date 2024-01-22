/* write.c - write */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

write(d,buf,nbytes)
int d;
char *buf;
int nbytes;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init)
	return(sys_write(check_fd(d),buf,nbytes));


    sys_hdr.sys_pid		= currpid;
    sys_hdr.sys_command		= SYS_write;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= d;
    sys_hdr.sys_val_len[1] 	= nbytes;
    sys_hdr.sys_val_len[2] 	= nbytes;

    proctab[currpid].sys_command = SYS_write;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
	sys_write(SYS_PIPE,buf,nbytes);
    ssignal(sem_sys_write);

    swait(proctab[currpid].sys_sem);

    proctab[currpid].sys_command = SYS_null;

    return(proctab[currpid].sys_ret_val);
}
