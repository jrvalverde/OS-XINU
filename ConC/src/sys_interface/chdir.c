/* chdir.c - chdir */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
 
/*
 *  This is done both in the ConC system and in the io_server.
 *  This way, some system calls can be done directly by ConC system.
 */

chdir(path)
char *path;
{
    struct sys_hdr sys_hdr;

    if (is_serv_init) {

	    sys_hdr.sys_pid		= currpid;
	    sys_hdr.sys_command		= SYS_chdir;
	    sys_hdr.sys_ret_val 	= 0;
	    sys_hdr.sys_errno		= errno;
	    sys_hdr.sys_val_len[0] 	= strlen(path) + 1;

	    proctab[currpid].sys_command = SYS_chdir;

	    swait(sem_sys_write);
		sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
		sys_write(SYS_PIPE,path,sys_hdr.sys_val_len[0]);
	    ssignal(sem_sys_write);

	    swait(proctab[currpid].sys_sem);

	    proctab[currpid].sys_command = SYS_null;
	    }

    if ((sys_hdr.sys_ret_val = sys_chdir(path)) < 0)
	return(SYSERR);
    else {
	getwd(cwd);
	return(sys_hdr.sys_ret_val);
	}
}
