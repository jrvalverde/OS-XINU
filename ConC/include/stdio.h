/* stdio.h */

/*
 *	Modified from original source
 *
 *	#define's for stdio routines deleted in user program because
 *	mutual exclusion of io buffers needed.
 *
 *	fd stuff
 */

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)stdio.h	5.3 (Berkeley) 3/15/86
 */

#define	BUFSIZ	1024
# ifndef FILE
extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;		/* should be unsigned char */
	char	*_base;		/* ditto */
	int	_bufsiz;
	short	_flag;
	char	_file;		/* should be short */
} _iob[];

#define	_IOREAD	01
#define	_IOWRT	02
#define	_IONBF	04
#define	_IOMYBUF	010
#define	_IOEOF	020
#define	_IOERR	040
#define	_IOSTRG	0100
#define	_IOLBF	0200
#define	_IORW	0400

#ifndef NULL
#define	NULL	(char *)0
#endif

#define	FILE	struct _iobuf
#define	EOF	(-1)

#define	stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define	stderr	(&_iob[2])
#ifndef lint
#define	stdio_getc(p)		(--(p)->_cnt>=0? (int)(*(unsigned char *)(p)->_ptr++):_filbuf(p))
#endif not lint
#define	stdio_getchar()	stdio_getc(stdin)
#ifndef lint
#define stdio_putc(x, p)	(--(p)->_cnt >= 0 ?\
	(int)(*(unsigned char *)(p)->_ptr++ = (x)) :\
	(((p)->_flag & _IOLBF) && -(p)->_cnt < (p)->_bufsiz ?\
		((*(p)->_ptr = (x)) != '\n' ?\
			(int)(*(unsigned char *)(p)->_ptr++) :\
			_flsbuf(*(unsigned char *)(p)->_ptr, p)) :\
		_flsbuf((unsigned char)(x), p)))
#endif not lint
#define	stdio_putchar(x)	stdio_putc(x,stdout)
#define	stdio_feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	stdio_ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	stdio_fileno(p)	((p)->_file)
#define	stdio_clearerr(p)	((p)->_flag &= ~(_IOERR|_IOEOF)) /* not used in CCC */

FILE	*fopen();
FILE	*fdopen();
FILE	*freopen();
FILE	*popen();
long	ftell();
char	*fgets();
char	*gets();
#ifdef vax
char	*sprintf();		/* too painful to do right */
#endif

/* fd stuff */

struct fd_ent {
	int			sem;		/* exclusive access sem	*/
	struct fd_request	*pfd_head;	/* head of request queue*/
	struct fd_request	*pfd_tail;	/* tail of request queue*/
};

extern struct fd_ent	*fd_tab;		/* fd table		*/

#define Get_Fd_Index(iop)	((FILE *)iop - &_iob[0])
#define stdio_swait(iop)	swait(fd_tab[Get_Fd_Index(iop)].sem)
#define stdio_ssignal(iop)	ssignal(fd_tab[Get_Fd_Index(iop)].sem)

#define issysfd(x)	((x) == ConC_SYS_PIPE || (x) == Serv_SYS_PIPE)
						/* check fd for sys pipe*/
#define isbadfd(x)	((x) < 0 || (x) > FD_SETSIZE || issysfd(x))
						/* check fd range	*/
#define check_fd(x)	(issysfd(x) ? BAD_DESC : (x))
						/* fd to pass to call	*/

/* FD constants */

#define STDIN		0		/* standard input		*/
#define STDOUT		1		/* standard output		*/
#define STDERR		2		/* standard error		*/
#define FD_NULL		(struct fd_request *)0
					/* sys_hdr NULL ptr		*/
#define BAD_DESC	-1		/* bad descriptor to pass for	*/
					/* system fd's			*/
# endif
