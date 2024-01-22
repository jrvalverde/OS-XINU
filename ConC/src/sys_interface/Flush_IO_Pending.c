/* Flush_IO_Pending.c - Flush_IO_Pending */

#include <kernel.h>
#include <syscall.h>
#include <proc.h>
#include <stdio.h>
#include <errno.h>

Flush_IO_Pending(tid)
int tid;
{
    struct sys_hdr sys_hdr;

    if (!is_serv_init) {
	return;
	}

    sys_hdr.sys_pid		= tid;
    sys_hdr.sys_command		= SYS_flush_io_pending;
    sys_hdr.sys_ret_val 	= 0;
    sys_hdr.sys_errno		= errno;
    sys_hdr.sys_val_len[0] 	= proctab[tid].sys_command;

    swait(sem_sys_write);
	sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    ssignal(sem_sys_write);
}
