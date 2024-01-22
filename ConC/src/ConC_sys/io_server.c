/* io_server.c - io_server, Enqueue_Req, Empty_Queue, Do_*, Dequeue_IO_Pending*/

#include <kernel.h>
#include <syscall.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

/****************************************************************
 *								*
 *	The way I handle updating the fd table is like this:	*
 *								*
 *	The sys_interface routines already have to update the	*
 *	table because of the defered io_server creation, so	*
 *	the io_server calls those routines as though they were	*
 *	the system calls to UNIX.  The fd table is updated	*
 *	almost as a side effect.  This can be done by having	*
 *	is_serv_init set false.					*
 *								*
 *	All IO calls are done on non-blocking descriptors.	*
 *	Because csh doesn't like its descriptors to be		*
 *	non-blocking, I first disable interrupts, set the fd	*
 *	for non-blocking, do the call, reset to blocking, then	*
 *	restore interrupts.  When the main ConcurrenC system	*
 *	exits (finishes on its own, or SIGINT or SIGQUIT is	*
 *	caught), SIGIO is sent to io_server, which is disabled	*
 *	during the IO call, so that io_server is not killed	*
 *	while a fd is in non-blocking mode.  The only way this	*
 *	should not work is if SIGKILL is sent when an IO call	*
 *	is being done, in which case the fd will still be in	*
 *	non-blocking mode.  If this fd is STDIN, STDOUT, or	*
 *	STDERR, the user's login will be terminated.		*
 *								*
 *	To see what would happen in that situation, compile	*
 *	and run the one-line program:				*
 *		main() { fcntl(0,4,4); }			*
 *	Be careful that you don't have anything you want to	*
 *	keep in background, because this program will log you	*
 *	off.							*
 *								*
 ****************************************************************/

#define CO_BLOCK	-1
#define CO_OK		 0

#define CON_1_TIME	 1
#define CON_2_TIME	 2

#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))

int	ppid;

struct sys_hdr sys_hdr;

struct fd_request {
	struct sys_hdr		sys_hdr;
	char			*data;
	struct fd_request	*pfd_request;
};

struct buf_sock {
	char			*buf;
	struct sockaddr		*addr;
};

/*-------------------------------------------------------------------------
 * io_server - Get IO requests from ConC system, handle them asynchronously,
 *	       and send back the results.
 *-------------------------------------------------------------------------
 */
io_server()
{
    int fd, nfound, nfds;
    fd_set readfds, save_readfds, writefds, save_writefds, exceptfds;
    char *malloc();

/* initialize select stuff, then go to it */

    ppid	  = sys_getppid();
    nfds	  = SYS_PIPE + 1;
    FD_ZERO(&save_readfds);
    FD_ZERO(&save_writefds);
    FD_SET(SYS_PIPE, &save_readfds);
    
    for (;;) {
	readfds	 = save_readfds;
	writefds = save_writefds;
	FD_ZERO(&exceptfds);

	if ((nfound = sys_select(nfds,&readfds,&writefds,&exceptfds,NULL)) < 0)
	    perror("server"), panic("error on sys_select");

	if (FD_ISSET(SYS_PIPE, &readfds)) {	/* get request from ConC sys */
	    FD_CLR(SYS_PIPE, &readfds);

	    Read_From_Pipe(&sys_hdr,sizeof(sys_hdr));

	    errno = sys_hdr.sys_errno;
	    sys_hdr.sys_ret_val = 0;

	    switch (sys_hdr.sys_command) {
		case SYS_flush_io_pending:
			Dequeue_IO_Pending(sys_hdr.sys_pid,
					sys_hdr.sys_val_len[0],
					&nfds,&save_readfds,&save_writefds);
		    break;

		case SYS_open:
		    {	int	len_path= sys_hdr.sys_val_len[0],
				flag	= sys_hdr.sys_val_len[1],
				mode	= sys_hdr.sys_val_len[2],
				fd;
		    	char	*path	= MALLOC(char *,len_path);

			Read_From_Pipe(path,len_path);

			sys_hdr.sys_ret_val	= fd = open(path,flag,mode);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			FREE(char *,path);
		    break;
		    }

		case SYS_close:
		    {	int			fd = sys_hdr.sys_val_len[0];

			sys_hdr.sys_ret_val	= close(fd);
			sys_hdr.sys_errno	= errno;

			if (sys_hdr.sys_ret_val >= 0)
			    Flush_Req(fd,2,&nfds,&save_readfds,&save_writefds);

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_read:
		    {	int	fd	= sys_hdr.sys_val_len[0],
				len_buf	= sys_hdr.sys_val_len[1];
			char	*buf	= MALLOC(char *,len_buf);

			if (fd_tab[fd].pfd_head != FD_NULL
			    || Do_read(fd,buf,len_buf) == CO_BLOCK)

			    Enqueue_Req(fd,buf,&nfds,&save_readfds);

			break;
		    }

		case SYS_write:
		    {	int	fd	= sys_hdr.sys_val_len[0],
				len_buf	= sys_hdr.sys_val_len[1];
			char	*buf	= MALLOC(char *,len_buf);

			Read_From_Pipe(buf,len_buf);

			if (fd_tab[fd].pfd_head != FD_NULL
			    || Do_write(fd,buf,len_buf) == CO_BLOCK)

			    Enqueue_Req(fd,buf,&nfds,&save_writefds);

			break;
		    }

		case SYS_fstat:
		    {   int	fd	= sys_hdr.sys_val_len[0],
				buf_len = sys_hdr.sys_val_len[1];
			char	*pbuf	= MALLOC(char *,buf_len);

			sys_hdr.sys_ret_val	= fstat(fd,pbuf);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			if (sys_hdr.sys_ret_val >= 0)
			    sys_write(SYS_PIPE,pbuf,buf_len);

			FREE(char *,pbuf);

			break;
		    }

		case SYS_ioctl:
		    {   int	fd	= sys_hdr.sys_val_len[0],
				req	= sys_hdr.sys_val_len[1],
				buf_len = sys_hdr.sys_val_len[2];
			char	*pbuf	= MALLOC(char *,buf_len);

			Read_From_Pipe(pbuf,buf_len);

			sys_hdr.sys_ret_val	= ioctl(fd,req,pbuf);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			if (sys_hdr.sys_ret_val >= 0)
			    sys_write(SYS_PIPE,pbuf,buf_len);

			FREE(char *,pbuf);

			break;
		    }

		case SYS_pipe:
		    {	int	fildes[2];

			sys_hdr.sys_ret_val	= pipe(fildes);
			sys_hdr.sys_errno	= errno;
			sys_hdr.sys_val_len[0] = fildes[0];
			sys_hdr.sys_val_len[1] = fildes[1];

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_dup:
		    {	int	oldd	= sys_hdr.sys_val_len[0],
				newd;

			sys_hdr.sys_ret_val	= newd = dup(oldd);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_dup2:
		    {	int	oldd	= sys_hdr.sys_val_len[0],
				newd	= sys_hdr.sys_val_len[1];

			sys_hdr.sys_ret_val	= dup2(oldd,newd);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_lseek:
		    {	int	d	= sys_hdr.sys_val_len[0],
				offset	= sys_hdr.sys_val_len[1],
				whence	= sys_hdr.sys_val_len[2];

			sys_hdr.sys_ret_val	= lseek(d,offset,whence);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_fsync:
		    {	int	fd	= sys_hdr.sys_val_len[0];

			sys_hdr.sys_ret_val	= fsync(fd);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_ftruncate:
		    {	int	fd	= sys_hdr.sys_val_len[0],
				length	= sys_hdr.sys_val_len[1];

			sys_hdr.sys_ret_val	= ftruncate(fd,length);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_exit:
		    {	int	code	= sys_hdr.sys_val_len[0];

			_exit(code);

		    break;
		    }

		case SYS_creat:
		    {	int	len_name= sys_hdr.sys_val_len[0],
				mode	= sys_hdr.sys_val_len[1],
				fd;
		    	char	*name	= MALLOC(char *,len_name);

			Read_From_Pipe(name,len_name);

			sys_hdr.sys_ret_val	= fd = creat(name,mode);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			FREE(char *,name);
		    break;
		    }

		case SYS_accept:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			addrlen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*addr	= MALLOC(struct sockaddr *,
								   addrlen);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_accept(s, addr, &addrlen) == CO_BLOCK)

			    Enqueue_Req(s,addr,&nfds,&save_readfds);

			break;
		    }

		case SYS_bind:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			namelen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*name	= MALLOC(struct sockaddr *,
								    namelen);

			Read_From_Pipe(name,namelen);

			sys_hdr.sys_ret_val	= bind(s, name, namelen);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			FREE(struct sockaddr *,name);

			break;
		    }

		case SYS_chdir:
		    {	int	pathlen	= sys_hdr.sys_val_len[0];
			char	*path	= MALLOC(char *,pathlen);

			Read_From_Pipe(path,pathlen);

			sys_hdr.sys_ret_val	= chdir(path);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			FREE(char *,path);

			break;
		    }

		case SYS_connect:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			namelen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*name	= MALLOC(struct sockaddr *,
								    namelen);

			Read_From_Pipe(name,namelen);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_connect(s, name, namelen, CON_1_TIME)
								== CO_BLOCK)

			    Enqueue_Req(s,name,&nfds,&save_writefds);

			break;
		    }

		case SYS_fchmod:
		    {	int		fd	= sys_hdr.sys_val_len[0],
		    			mode	= sys_hdr.sys_val_len[1];

			sys_hdr.sys_ret_val	= fchmod(fd, mode);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			break;
		    }

		case SYS_fchown:
		    {	int		fd	= sys_hdr.sys_val_len[0],
		    			owner	= sys_hdr.sys_val_len[1],
		    			group	= sys_hdr.sys_val_len[2];

			sys_hdr.sys_ret_val	= fchown(fd, owner, group);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			break;
		    }

		case SYS_getpeername:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			namelen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*name	= MALLOC(struct sockaddr *,
								    namelen);

			sys_hdr.sys_ret_val	= getpeername(s,name,&namelen);
			sys_hdr.sys_errno	= errno;
			sys_hdr.sys_val_len[1]	= namelen;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			if (sys_hdr.sys_ret_val >= 0)
			    sys_write(SYS_PIPE,(char *)name,namelen);

			FREE(struct sockaddr *,name);

			break;
		    }

		case SYS_getsockname:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			namelen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*name	= MALLOC(struct sockaddr *,
								    namelen);

			sys_hdr.sys_ret_val	= getsockname(s,name,&namelen);
			sys_hdr.sys_errno	= errno;
			sys_hdr.sys_val_len[1]	= namelen;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			if (sys_hdr.sys_ret_val >= 0)
			    sys_write(SYS_PIPE,(char *)name,namelen);

			FREE(struct sockaddr *,name);

			break;
		    }

		case SYS_getsockopt:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			level	= sys_hdr.sys_val_len[1],
		    			optname	= sys_hdr.sys_val_len[2],
		    			optlen	= sys_hdr.sys_val_len[3];
			char		*optval	= MALLOC(char *,optlen);

			sys_hdr.sys_ret_val	=
				getsockopt(s, level, optname, optval, &optlen);
			sys_hdr.sys_errno	= errno;
			sys_hdr.sys_val_len[3]	= optlen;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			if (sys_hdr.sys_ret_val >= 0)
			    sys_write(SYS_PIPE,(char *)optval,optlen);

			FREE(char *,optval);

			break;
		    }

		case SYS_listen:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			backlog	= sys_hdr.sys_val_len[1];

			sys_hdr.sys_ret_val	= listen(s, backlog);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			break;
		    }

		case SYS_readv:
		    {	int	d	= sys_hdr.sys_val_len[0],
				iovcnt	= sys_hdr.sys_val_len[1],
				i;
			struct iovec *iov = (struct iovec *)
					malloc(iovcnt * sizeof(struct iovec));

			for ( i = 0 ; i < iovcnt ; i++ ) {
			    Read_From_Pipe(&iov[i].iov_len,sizeof(int));
			    iov[i].iov_base = MALLOC(char *,iov[i].iov_len);
			    }

			if (fd_tab[d].pfd_head != FD_NULL
			    || Do_readv(d,iov,iovcnt) == CO_BLOCK)

			    Enqueue_Req(d,iov,&nfds,&save_readfds);

			break;
		    }

		case SYS_recv:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2];
			char	*buf	= MALLOC(char *,len);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_recv(s, buf, len, flags) == CO_BLOCK)

			    Enqueue_Req(s,buf,&nfds,&save_readfds);

			break;
		    }

		case SYS_recvfrom:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2],
				fromlen	= sys_hdr.sys_val_len[3];
			char	*buf	= MALLOC(char *,len);
			struct sockaddr	*from = MALLOC(struct sockaddr *,
								   fromlen);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_recvfrom(s,buf,len,flags,from,&fromlen)
								== CO_BLOCK) {

			    struct buf_sock *recv_str = (struct buf_sock *)
						malloc(sizeof(struct buf_sock));

			    recv_str -> buf	= buf;
			    recv_str -> addr	= from;
			    Enqueue_Req(s,recv_str,&nfds,&save_readfds);
			    }

			break;
		    }

		case SYS_recvmsg:
		    {	int	s	= sys_hdr.sys_val_len[0],
				flags	= sys_hdr.sys_val_len[1],
				iovcnt	= sys_hdr.sys_val_len[2],
				accrlen	= sys_hdr.sys_val_len[3],
				i;
			struct iovec *iov = (struct iovec *)
					malloc(iovcnt * sizeof(struct iovec));
			struct msghdr *msg = (struct msghdr *)
						malloc(sizeof(struct msghdr));

			msg -> msg_name		= NULL;
			msg -> msg_namelen	= 0;
			msg -> msg_iov		= iov;
			msg -> msg_iovlen	= iovcnt;
			msg -> msg_accrights	= MALLOC(char *,accrlen);
			msg -> msg_accrightslen	= accrlen;

			for ( i = 0 ; i < iovcnt ; i++ ) {
			    Read_From_Pipe(&iov[i].iov_len,sizeof(int));
			    iov[i].iov_base = MALLOC(char *,iov[i].iov_len);
			    }

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_recvmsg(s, msg, flags) == CO_BLOCK)

			    Enqueue_Req(s,msg,&nfds,&save_readfds);

			break;
		    }

		case SYS_send:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2];
			char	*msg	= MALLOC(char *,len);

			Read_From_Pipe(msg,len);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_send(s, msg, len, flags) == CO_BLOCK)

			    Enqueue_Req(s,msg,&nfds,&save_writefds);

			break;
		    }

		case SYS_sendmsg:		/* make into one iov block */
		    {	int	s	= sys_hdr.sys_val_len[0],
				flags	= sys_hdr.sys_val_len[1],
				namelen	= sys_hdr.sys_val_len[2],
				iovlen	= sys_hdr.sys_val_len[3],
				accrlen	= sys_hdr.sys_val_len[4],
				where;
			struct msghdr	*msg = (struct msghdr *)
						malloc(sizeof(struct msghdr));
			struct iovec   *iov = (struct iovec *)
						malloc(sizeof(struct iovec));

			msg -> msg_namelen = namelen;
			msg -> msg_name = MALLOC(char *,msg -> msg_namelen);
			Read_From_Pipe(msg -> msg_name,msg -> msg_namelen);

			msg -> msg_iov = iov;
			Read_From_Pipe(&msg -> msg_iov[0].iov_len,sizeof(int));
			msg -> msg_iov[0].iov_base =
					MALLOC(char *,msg->msg_iov[0].iov_len);
			for ( where = 0 ; where < msg -> msg_iov[0].iov_len ; )
			      where += Read_From_Pipe(
					&msg -> msg_iov[0].iov_base[where],
					 msg -> msg_iov[0].iov_len - where);
			msg -> msg_iovlen = 1;

			msg -> msg_accrightslen = accrlen;
			msg -> msg_accrights = MALLOC(char *,
						msg->msg_accrightslen);
			Read_From_Pipe(msg -> msg_accrights,
						msg -> msg_accrightslen);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_sendmsg(s, msg, flags) == CO_BLOCK)

			    Enqueue_Req(s,msg,&nfds,&save_writefds);

			break;
		    }

		case SYS_sendto:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2],
				tolen	= sys_hdr.sys_val_len[3];
			char	*msg	= MALLOC(char *,len);
			struct sockaddr *to = MALLOC(struct sockaddr *,tolen);

			Read_From_Pipe(msg,len);
			Read_From_Pipe(to,tolen);

			if (fd_tab[s].pfd_head != FD_NULL
			    || Do_sendto(s,msg,len,flags,to,tolen) == CO_BLOCK){

			    struct buf_sock *send_str = (struct buf_sock *)
						malloc(sizeof(struct buf_sock));

			    send_str -> buf	= msg;
			    send_str -> addr	= to;
			    Enqueue_Req(s,send_str,&nfds,&save_writefds);
			    }

			break;
		    }

		case SYS_setsockopt:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			level	= sys_hdr.sys_val_len[1],
		    			optname	= sys_hdr.sys_val_len[2],
		    			optlen	= sys_hdr.sys_val_len[3];
			char		*optval	= MALLOC(char *,optlen);

			Read_From_Pipe(optval,optlen);

			sys_hdr.sys_ret_val	=
				setsockopt(s, level, optname, optval, optlen);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			FREE(char *,optval);

			break;
		    }

		case SYS_shutdown:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			how	= sys_hdr.sys_val_len[1];

			sys_hdr.sys_ret_val	= shutdown(s, how);
			sys_hdr.sys_errno	= errno;

			if (sys_hdr.sys_ret_val >= 0)
			    Flush_Req(fd,how,
					&nfds,&save_readfds,&save_writefds);

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

			break;
		    }

		case SYS_socket:
		    {	int	af		= sys_hdr.sys_val_len[0],
				type		= sys_hdr.sys_val_len[1],
				protocol	= sys_hdr.sys_val_len[2],
				fd;

			sys_hdr.sys_ret_val	= fd = socket(af,type,protocol);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_socketpair:
		    {	int	d		= sys_hdr.sys_val_len[0],
				type		= sys_hdr.sys_val_len[1],
				protocol	= sys_hdr.sys_val_len[2],
				sv[2];

			sys_hdr.sys_ret_val    = socketpair(d,type,protocol,sv);
			sys_hdr.sys_errno      = errno;
			sys_hdr.sys_val_len[3] = sv[0];
			sys_hdr.sys_val_len[4] = sv[1];

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_sync:
		    {
			sys_hdr.sys_ret_val    = sync();
			sys_hdr.sys_errno      = errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_umask:
		    {	int	numask	= sys_hdr.sys_val_len[0];

			sys_hdr.sys_ret_val	= umask(numask);
			sys_hdr.sys_errno	= errno;

			sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
			sys_kill(ppid,SIGIO);

		    break;
		    }

		case SYS_writev:		/* make into one iov block */
		    {	int	d	= sys_hdr.sys_val_len[0],
				iovlen	= sys_hdr.sys_val_len[1],
				where;
			struct iovec   *iov = (struct iovec *)
						malloc(sizeof(struct iovec));

			Read_From_Pipe(&iov[0].iov_len,sizeof(int));

			iov[0].iov_base = MALLOC(char *,iov[0].iov_len);
			
			for ( where = 0 ; where < iov[0].iov_len ; )
			      where += Read_From_Pipe(&iov[0].iov_base[where],
							iov[0].iov_len - where);

			if (fd_tab[d].pfd_head != FD_NULL
			    || Do_writev(d, iov, 1) == CO_BLOCK)

			    Enqueue_Req(d,iov,&nfds,&save_writefds);

			break;
		    }

		default:
	kprintf_serv("Warning:\tServer does not recognize command #%d.\n",
							   sys_hdr.sys_command);
		}
	    }

	for ( fd = 0 ; fd < nfds ; fd++ )
	    if (FD_ISSET(fd, &writefds)) {		   /* ready for write */
		FD_CLR(fd, &writefds);
		Empty_Queue(fd,&nfds,&save_readfds,&save_writefds);
		}

	for ( fd = 0 ; fd < nfds ; fd++ )
	    if (FD_ISSET(fd, &readfds)) {		   /* ready for read  */
		FD_CLR(fd, &readfds);
		Empty_Queue(fd,&nfds,&save_readfds,&save_writefds);
		}
	}
}

/*-------------------------------------------------------------------------
 * Enqueue_Req - Enqueue a request for later service (when fd is ready).
 *-------------------------------------------------------------------------
 */
LOCAL
Enqueue_Req(fd,buf,pnfds,pfds)
int		fd, *pnfds;
char		*buf;
fd_set		*pfds;
{
    struct fd_request	*pfd_req = (struct fd_request *)
					      malloc(sizeof(struct fd_request));

    pfd_req -> sys_hdr	= sys_hdr;
    pfd_req -> data	= buf;
    pfd_req -> pfd_request = FD_NULL;
    if (fd_tab[fd].pfd_head == FD_NULL) {
	fd_tab[fd].pfd_head = fd_tab[fd].pfd_tail = pfd_req;
	FD_SET(fd, pfds);
	*pnfds = max(*pnfds,fd+1);
	}
    else
	fd_tab[fd].pfd_tail = fd_tab[fd].pfd_tail -> pfd_request = pfd_req;
}

/*-------------------------------------------------------------------------
 * Empty_Queue - Empty the defered IO queue for this fd until either it
 *		 would block again or it is empty.
 *-------------------------------------------------------------------------
 */
LOCAL
Empty_Queue(fd,pnfds,preadfds,pwritefds)
int fd, *pnfds;
fd_set *preadfds, *pwritefds;
{
    int done, fd_trans;
    struct fd_request *pfd_req, *old_pfd_req;

    for ( done = FALSE, pfd_req = fd_tab[fd].pfd_head
					; !done && pfd_req != FD_NULL ; ) {

	sys_hdr	= pfd_req -> sys_hdr;
	errno	= sys_hdr.sys_errno;

	switch (sys_hdr.sys_command) {
		case SYS_read:
		    {	int	fd	= sys_hdr.sys_val_len[0],
				len_buf	= sys_hdr.sys_val_len[1];
			char	*buf	= pfd_req -> data;

			if (Do_read(fd,buf,len_buf) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_readv:
		    {	int	d	= sys_hdr.sys_val_len[0],
				iovcnt	= sys_hdr.sys_val_len[1];
			struct iovec *iov = (struct iovec *) pfd_req -> data;

			if (Do_readv(d,iov,iovcnt) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_accept:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			addrlen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*addr	= (struct sockaddr *)
								pfd_req -> data;

			if (Do_accept(s, addr, &addrlen) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_recv:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2];
			char	*buf	= pfd_req -> data;

			if (Do_recv(s, buf, len, flags) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_recvfrom:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2],
				fromlen	= sys_hdr.sys_val_len[3];
			struct buf_sock *recv_str = (struct buf_sock *)
							pfd_req -> data;
			char	*buf		  = recv_str -> buf;
			struct sockaddr	*from	  = recv_str -> addr;

			if (Do_recvfrom(s,buf,len,flags,from,&fromlen)
								== CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_recvmsg:
		    {	int	s	= sys_hdr.sys_val_len[0],
				flags	= sys_hdr.sys_val_len[1],
				iovcnt	= sys_hdr.sys_val_len[2],
				accrlen	= sys_hdr.sys_val_len[3];
			struct msghdr *msg = (struct msghdr *) pfd_req -> data;

			if (Do_recvmsg(s, msg, flags) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_write:
		    {	int	fd	= sys_hdr.sys_val_len[0],
				len_buf	= sys_hdr.sys_val_len[1];
			char	*buf	= pfd_req -> data;

			if (Do_write(fd,buf,len_buf) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_writev:
		    {	int	d	= sys_hdr.sys_val_len[0],
				iovlen	= sys_hdr.sys_val_len[1];
			struct iovec   *iov = (struct iovec *) pfd_req -> data;

			if (Do_writev(d, iov, 1) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_connect:
		    {	int		s	= sys_hdr.sys_val_len[0],
		    			namelen	= sys_hdr.sys_val_len[1];
			struct sockaddr	*name	= (struct sockaddr *)
								pfd_req -> data;

			if (Do_connect(s, name, namelen,CON_2_TIME) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_send:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2];
			char	*msg	= pfd_req -> data;

			if (Do_send(s, msg, len, flags) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_sendto:
		    {	int	s	= sys_hdr.sys_val_len[0],
				len	= sys_hdr.sys_val_len[1],
				flags	= sys_hdr.sys_val_len[2],
				tolen	= sys_hdr.sys_val_len[3];
			struct buf_sock *send_str = (struct buf_sock *)
								pfd_req -> data;
			char	*msg		  = send_str -> buf;
			struct sockaddr *to	  = send_str -> addr;

			if (Do_sendto(s,msg,len,flags,to,tolen) == CO_BLOCK)
			    done = TRUE;

			break;
		    }

		case SYS_sendmsg:
		    {	int	s	= sys_hdr.sys_val_len[0],
				flags	= sys_hdr.sys_val_len[1],
				namelen	= sys_hdr.sys_val_len[2],
				iovlen	= sys_hdr.sys_val_len[3],
				accrlen	= sys_hdr.sys_val_len[4];
			struct msghdr *msg = (struct msghdr *) pfd_req -> data;

			if (Do_sendmsg(s, msg, flags) == CO_BLOCK)
			    done = TRUE;

			break;
		    }
	    }

	if (!done) {
	    old_pfd_req = pfd_req;
	    pfd_req = pfd_req -> pfd_request;
	    Free_Req(old_pfd_req);
	    }
	}

    if ((fd_tab[fd].pfd_head = pfd_req) == FD_NULL) {
	fd_tab[fd].pfd_tail = FD_NULL;
	FD_CLR(fd, preadfds);
	FD_CLR(fd, pwritefds);
	if (*pnfds == fd+1) {
	    for ( fd_trans = fd-1 ;
		  !FD_SET(fd_trans, preadfds) && !FD_SET(fd_trans, pwritefds) ;
		  fd_trans-- )
		/* NULL */;
	    *pnfds = fd_trans + 1;
	    }
	}
    else
	switch (pfd_req -> sys_hdr.sys_command) {
		case SYS_read:
		case SYS_readv:
		case SYS_accept:
		case SYS_recv:
		case SYS_recvfrom:
		case SYS_recvmsg:	FD_SET(fd, preadfds);
					FD_CLR(fd, pwritefds);
					break;

		case SYS_write:
		case SYS_writev:
		case SYS_connect:
		case SYS_send:
		case SYS_sendto:
		case SYS_sendmsg:	FD_CLR(fd, preadfds);
					FD_SET(fd, pwritefds);
					break;
		}
}

/*-------------------------------------------------------------------------
 * Do_* (where * is a system call that might block) -
 *	disable interrupts,
 *	set file descriptor non-blocking
 *	try request
 *	set file descriptor blocking again
 *	enable a selected group of interrupts
 *
 *	if call would block, return such info
 *	else send results back to ConC system and return OK flag
 *-------------------------------------------------------------------------
 */
LOCAL
Do_read(fd,buf,len_buf)
int	fd;
char	*buf;
int	len_buf;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(fd,F_SETFL,FNDELAY);
    ret_val	= read(fd,buf,len_buf);
    ret_errno	= errno;
    sys_fcntl(fd,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    sys_hdr.sys_ret_val	= ret_val;
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    if (ret_val > 0)
        sys_write(SYS_PIPE,buf,ret_val);

    return(CO_OK);
}

LOCAL
Do_write(fd,buf,len_buf)
int	fd;
char	*buf;
int	len_buf;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(fd,F_SETFL,FNDELAY);
    ret_val	= write(fd,buf,len_buf);
    ret_errno	= errno;
    sys_fcntl(fd,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    if (0 < ret_val && ret_val < len_buf) {	/* must write all bytes.  If not
						 * all written, copy remaining
						 * bytes to begining of buffer.
						 */
	bcopy(&buf[ret_val],&buf[0],len_buf - ret_val);
	sys_hdr.sys_ret_val 	+= ret_val;
	sys_hdr.sys_val_len[1]	-= ret_val;

	return(CO_BLOCK);
	}

    sys_hdr.sys_ret_val	= ret_val + ((ret_val > 0) ? sys_hdr.sys_ret_val : 0);
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    return(CO_OK);
}

LOCAL
Do_accept(s, addr, paddrlen)
int		s;
struct sockaddr *addr;
int		*paddrlen;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= accept(s, addr, paddrlen);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    sys_hdr.sys_ret_val		= ret_val;
    sys_hdr.sys_errno		= ret_errno;
    sys_hdr.sys_val_len[1]	= *paddrlen;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    if (sys_hdr.sys_ret_val >= 0 && *paddrlen > 0)
	sys_write(SYS_PIPE,(char *)addr,*paddrlen);

    return(CO_OK);
}

LOCAL
Do_connect(s, name, namelen, which_time)
int		s;
struct sockaddr *name;
int		namelen, which_time;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= connect(s, name, namelen);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && (ret_errno == EWOULDBLOCK || ret_errno == EINPROGRESS))
	return(CO_BLOCK);

    if (which_time == CON_2_TIME && ret_val < 0 && ret_errno == EISCONN)
	sys_hdr.sys_ret_val = 0;
    else {
	sys_hdr.sys_ret_val	= ret_val;
	sys_hdr.sys_errno	= ret_errno;
	}

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    return(CO_OK);
}

LOCAL
Do_readv(d,iov,iovcnt)
int		d;
struct iovec	*iov;
int		iovcnt;
{
    int i, cc_togo, ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(d,F_SETFL,FNDELAY);
    ret_val	= readv(d,iov,iovcnt);
    ret_errno	= errno;
    sys_fcntl(d,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    sys_hdr.sys_ret_val	= cc_togo = ret_val;
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    for ( i = 0 ; cc_togo > 0 ; i++ )
	cc_togo -= sys_write(SYS_PIPE,iov[i].iov_base,
				    min(cc_togo,iov[i].iov_len));

    return(CO_OK);
}

LOCAL
Do_recv(s, buf, len, flags)
int	s;
char	*buf;
int	len, flags;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= recv(s, buf, len, flags);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    sys_hdr.sys_ret_val	= ret_val;
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    if (sys_hdr.sys_ret_val >= 0)
	sys_write(SYS_PIPE,buf,ret_val);

    return(CO_OK);
}

LOCAL
Do_recvfrom(s,buf,len,flags,from,pfromlen)
int		s;
char		*buf;
int		len, flags;
struct sockaddr *from;
int		*pfromlen;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= recvfrom(s,buf,len,flags,from,pfromlen);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    sys_hdr.sys_ret_val	= ret_val;
    sys_hdr.sys_errno	= ret_errno;
    sys_hdr.sys_val_len[3] = *pfromlen;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    if (ret_val >= 0) {
	sys_write(SYS_PIPE,buf,ret_val);
	sys_write(SYS_PIPE,from,*pfromlen);
	}

    return(CO_OK);
}

LOCAL
Do_recvmsg(s, msg, flags)
int		s;
struct msghdr	*msg;
int		flags;
{
    int i, cc_togo, ret_val, ret_errno, iovcnt = msg -> msg_iovlen;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= recvmsg(s, msg, flags);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    sys_hdr.sys_ret_val	= cc_togo = ret_val;
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    for ( i = 0 ; cc_togo > 0 ; i++ )
	cc_togo -= sys_write(SYS_PIPE,msg -> msg_iov[i].iov_base,
			     min(cc_togo,msg -> msg_iov[i].iov_len));

    if (ret_val > 0)
	sys_write(SYS_PIPE,msg -> msg_accrights,msg -> msg_accrightslen);

    return(CO_OK);
}

LOCAL
Do_send(s, msg, len, flags)
int	s;
char	*msg;
int	len, flags;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= send(s, msg, len, flags);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    if (0 < ret_val && ret_val < len) {		/* must write all bytes.  If not
						 * all written, copy remaining
						 * bytes to begining of buffer.
						 */

	bcopy(&msg[ret_val],&msg[0],len - ret_val);
	sys_hdr.sys_ret_val 	+= ret_val;
	sys_hdr.sys_val_len[1]	-= ret_val;

	return(CO_BLOCK);
	}

    sys_hdr.sys_ret_val	= ret_val + ((ret_val > 0) ? sys_hdr.sys_ret_val : 0);
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    return(CO_OK);
}

LOCAL
Do_sendmsg(s, msg, flags)
int		s;
struct msghdr	*msg;
int		flags;
{
    int i, ret_val, ret_errno, iovlen  = msg -> msg_iovlen;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= sendmsg(s, msg, flags);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    if (0 < ret_val && ret_val < msg -> msg_iov[0].iov_len) {
    						/* must write all bytes.  If not
						 * all written, copy remaining
						 * bytes to begining of buffer.
						 */

	bcopy(&msg -> msg_iov[0].iov_base[ret_val],
					&msg -> msg_iov[0].iov_base[0],
					 msg -> msg_iov[0].iov_len - ret_val);
	sys_hdr.sys_ret_val 	  += ret_val;
	msg -> msg_iov[0].iov_len -= ret_val;

	return(CO_BLOCK);
	}

    sys_hdr.sys_ret_val	= ret_val + ((ret_val > 0) ? sys_hdr.sys_ret_val : 0);
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    return(CO_OK);
}

LOCAL
Do_sendto(s,msg,len,flags,to,tolen)
int		s;
char		*msg;
int		len, flags;
struct sockaddr *to;
int		tolen;
{
    int ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(s,F_SETFL,FNDELAY);
    ret_val	= sendto(s,msg,len,flags,to,tolen);
    ret_errno	= errno;
    sys_fcntl(s,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    if (0 < ret_val && ret_val < len) {		/* must write all bytes.  If not
						 * all written, copy remaining
						 * bytes to begining of buffer.
						 */

	bcopy(&msg[ret_val],&msg[0],len - ret_val);
	sys_hdr.sys_ret_val 	+= ret_val;
	sys_hdr.sys_val_len[1]	-= ret_val;

	return(CO_BLOCK);
	}

    sys_hdr.sys_ret_val	= ret_val + ((ret_val > 0) ? sys_hdr.sys_ret_val : 0);
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    return(CO_OK);
}

LOCAL
Do_writev(d, iov, iovlen)
int		d;
struct iovec	*iov;
int		iovlen;
{
    int i, ret_val, ret_errno;
    long ps;

    serv_disable(ps);
    sys_fcntl(d,F_SETFL,FNDELAY);
    ret_val	= writev(d, iov, iovlen);
    ret_errno	= errno;
    sys_fcntl(d,F_SETFL,0);
    serv_restore(ps);

    if (ret_val < 0 && ret_errno == EWOULDBLOCK)
	return(CO_BLOCK);

    if (0 < ret_val && ret_val < iov[0].iov_len) {
    						/* must write all bytes.  If not
						 * all written, copy remaining
						 * bytes to begining of buffer.
						 */

	bcopy(&iov[0].iov_base[ret_val],&iov[0].iov_base[0],
						iov[0].iov_len - ret_val);
	sys_hdr.sys_ret_val 	+= ret_val;
	iov[0].iov_len		-= ret_val;

	return(CO_BLOCK);
	}

    sys_hdr.sys_ret_val	= ret_val + ((ret_val > 0) ? sys_hdr.sys_ret_val : 0);
    sys_hdr.sys_errno	= ret_errno;

    sys_write(SYS_PIPE,&sys_hdr,sizeof(sys_hdr));
    sys_kill(ppid,SIGIO);

    return(CO_OK);
}

LOCAL
Dequeue_IO_Pending(tid,command,pnfds,preadfds,pwritefds)
int tid, command, *pnfds;
fd_set *preadfds, *pwritefds;
{
    int fd, fd_trans;
    struct fd_request *pfd, *pfd_old;

    for (fd = 0 ; fd < FD_SETSIZE ; fd++) {

	if ((pfd = fd_tab[fd].pfd_head) == FD_NULL)
	    continue;

	if (pfd -> sys_hdr.sys_pid == tid) {
	    if (pfd -> sys_hdr.sys_command != command)
		kprintf("Warning:\tDifferent syscall command being flushed for tid %d than enqueued.\n", tid);
	    if ((fd_tab[fd].pfd_head = pfd -> pfd_request) == FD_NULL) {
		fd_tab[fd].pfd_tail = FD_NULL;
		FD_CLR(fd, preadfds);
		FD_CLR(fd, pwritefds);
		if (*pnfds == fd+1) {
		    for ( fd_trans = fd-1 ; !FD_SET(fd_trans, preadfds) &&
					    !FD_SET(fd_trans, pwritefds) ;
			  					fd_trans-- )
			/* NULL */;
		    *pnfds = fd_trans + 1;
		    }
		}
	    else
		switch (pfd -> sys_hdr.sys_command) {
			case SYS_read:
			case SYS_readv:
			case SYS_accept:
			case SYS_recv:
			case SYS_recvfrom:
			case SYS_recvmsg:	FD_SET(fd, preadfds);
						FD_CLR(fd, pwritefds);
						break;

			case SYS_write:
			case SYS_writev:
			case SYS_connect:
			case SYS_send:
			case SYS_sendto:
			case SYS_sendmsg:	FD_CLR(fd, preadfds);
						FD_SET(fd, pwritefds);
						break;
			}

	    Free_Req(pfd);

	    return;
	    }

	for (pfd_old = pfd, pfd = pfd_old -> pfd_request ; pfd != FD_NULL ;
				pfd_old = pfd, pfd = pfd_old -> pfd_request)
	    if (pfd -> sys_hdr.sys_pid == tid) {
		if (pfd -> sys_hdr.sys_command != command)
		    kprintf("Warning:\tDifferent syscall command being flushed for tid %d than enqueued.\n", tid);
		pfd_old -> pfd_request = pfd -> pfd_request;
		if (fd_tab[fd].pfd_tail = pfd)
		    fd_tab[fd].pfd_tail = pfd_old;
		Free_Req(pfd);

		return;
		}
	}

    kprintf("Warning:\tio_server tried to flush pending I/O for task %d\n",tid);
}

Flush_Req(fd, how, pnfds, preadfds, pwritefds)
int fd, how, *pnfds;
fd_set *preadfds, *pwritefds;
{
    int			fd_trans, delete;
    struct fd_request	*pfd_req, *pfd_del;
    struct sys_hdr	tmp_sys_hdr;

    if ((pfd_req = fd_tab[fd].pfd_head) == FD_NULL)
	return;

    fd_tab[fd].pfd_head = fd_tab[fd].pfd_tail = FD_NULL;
    for ( ; pfd_req != FD_NULL ; ) {
	switch (pfd_req -> sys_hdr.sys_command) {
		case SYS_read:
		case SYS_readv:
		case SYS_accept:
		case SYS_recv:
		case SYS_recvfrom:
		case SYS_recvmsg:	delete = (how == 0 || how == 2)
								? TRUE : FALSE;
					break;

		case SYS_write:
		case SYS_writev:
		case SYS_connect:
		case SYS_send:
		case SYS_sendto:
		case SYS_sendmsg:	delete = (how == 1 || how == 2)
								? TRUE : FALSE;
					break;
		}
	if (delete) {
	    tmp_sys_hdr.sys_pid		= pfd_req -> sys_hdr.sys_pid;
	    tmp_sys_hdr.sys_command	= pfd_req -> sys_hdr.sys_command;
	    tmp_sys_hdr.sys_ret_val	= SYSERR;
	    tmp_sys_hdr.sys_errno	= EBADF;

	    sys_write(SYS_PIPE,&tmp_sys_hdr,sizeof(sys_hdr));
	    sys_kill(ppid,SIGIO);

	    pfd_del = pfd_req;
	    pfd_req = pfd_req -> pfd_request;
	    Free_Req(pfd_del);
	    }
	else {
	    if (fd_tab[fd].pfd_head == FD_NULL)
		fd_tab[fd].pfd_head = pfd_req;
	    fd_tab[fd].pfd_tail = pfd_req;
	    pfd_req = pfd_req -> pfd_request;
	    }
	}

    if (fd_tab[fd].pfd_head == FD_NULL) {
	FD_CLR(fd, preadfds);
	FD_CLR(fd, pwritefds);
	if (*pnfds == fd+1) {
	    for ( fd_trans = fd-1 ; !FD_SET(fd_trans, preadfds) &&
				    !FD_SET(fd_trans, pwritefds) ;
				  				fd_trans-- )
		/* NULL */;
	    *pnfds = fd_trans + 1;
	    }
	}
    else
	switch (pfd_req -> sys_hdr.sys_command) {
		case SYS_read:
		case SYS_readv:
		case SYS_accept:
		case SYS_recv:
		case SYS_recvfrom:
		case SYS_recvmsg:	FD_SET(fd, preadfds);
					FD_CLR(fd, pwritefds);
					break;

		case SYS_write:
		case SYS_writev:
		case SYS_connect:
		case SYS_send:
		case SYS_sendto:
		case SYS_sendmsg:	FD_CLR(fd, preadfds);
					FD_SET(fd, pwritefds);
					break;
		}
}

Free_Req(pfd)
struct fd_request *pfd;
{
    int i;

    switch (pfd -> sys_hdr.sys_command) {
	case SYS_read:		FREE(char *, pfd -> data);
				break;

	case SYS_readv:		for ( i = 0 ;
					i < pfd -> sys_hdr.sys_val_len[1] ;i++ )
				    FREE(char *, ((struct iovec *)
						pfd -> data)[i].iov_base);
				FREE(struct iovec *,
					(struct iovec *) pfd -> data);
				break;

	case SYS_accept:	FREE(struct sockaddr *,
					(struct sockaddr *) pfd -> data);
				break;

	case SYS_recv:		FREE(char *, pfd -> data);
				break;

	case SYS_recvfrom:	FREE(char *,
				    ((struct buf_sock *) pfd -> data) -> buf);
				FREE(struct sockaddr *, (struct sockaddr *)
				    ((struct buf_sock *) pfd -> data) -> addr);
				FREE(struct buf_sock *,
				    (struct buf_sock *) pfd -> data);
				break;

	case SYS_recvmsg:	for ( i = 0 ; i < ((struct msghdr *)
					    pfd -> data) -> msg_iovlen ; i++ )
				    FREE(char *, ((struct msghdr *)
					pfd -> data) -> msg_iov[i].iov_base);
				FREE(struct iovec *, ((struct msghdr *)
						pfd -> data) -> msg_iov);
				FREE(char *, ((struct msghdr *)
						pfd -> data) -> msg_accrights);
				FREE(struct msghdr *,
					(struct msghdr *) pfd -> data);
				break;

	case SYS_write:		FREE(char *, pfd -> data);
				break;

	case SYS_writev:	for ( i = 0 ; i < 1 ; i++ )  /* bogus, I know */
				    FREE(char *, ((struct iovec *)
						pfd -> data)[i].iov_base);
				FREE(struct iovec *,
					(struct iovec *) pfd -> data);
				break;

	case SYS_connect:	FREE(struct sockaddr *,
					(struct sockaddr *) pfd -> data);
				break;

	case SYS_send:		FREE(char *, pfd -> data);
				break;

	case SYS_sendto:	FREE(char *,
				    ((struct buf_sock *) pfd -> data) -> buf);
				FREE(struct sockaddr *, (struct sockaddr *)
				    ((struct buf_sock *) pfd -> data) -> addr);
				FREE(struct buf_sock *,
				    (struct buf_sock *) pfd -> data);
				break;

	case SYS_sendmsg:	for ( i = 0 ; i < ((struct msghdr *)
						pfd -> data) -> msg_iovlen ;
						i++ )
				    FREE(char *, ((struct msghdr *)
					pfd -> data) -> msg_iov[i].iov_base);
				FREE(char *, ((struct msghdr *)
					pfd -> data) -> msg_name);
				FREE(struct iovec *, (struct iovec *)
				    ((struct msghdr *) pfd -> data) -> msg_iov);
				FREE(char *, ((struct msghdr *)
					pfd -> data) -> msg_accrights);
				FREE(struct msghdr *,
					(struct msghdr *) pfd -> data);
				break;

	}

    FREE(struct fd_request *, pfd);
}
