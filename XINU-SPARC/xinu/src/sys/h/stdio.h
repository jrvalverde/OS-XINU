/*	@(#)stdio.h	1.1 (Berkeley) 3/9/81	*/
/* Modified for Sun: BUFSIZE set to 512. Per Bothner 82/Jun/14 */
/* ifdef Vsystem hack: Bothner June 83 */
#ifdef Vsystem
#include <Vio.h>
#else
#define	BUFSIZ	512
#define	_NFILE	20
# ifndef FILE
extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	int	_bufsiz;
	short	_flag;
	char	_file;
} _iob[_NFILE];
# endif

#define	_IOREAD	01
#define	_IOWRT	02
#define	_IONBF	04
#define	_IOMYBUF	010
#define	_IOEOF	020
#define	_IOERR	040
#define	_IOSTRG	0100
#define	_IOLBF	0200
#define	_IORW	0400
#define	NULL	0
#define	FILE	struct _iobuf
#define	EOF	(-1)

#define	stdin	(&_iob[0])
#define	stdout	(&_iob[1])
#define	stderr	(&_iob[2])
#define	getc(p)		(--(p)->_cnt>=0? *(p)->_ptr++&0377:_filbuf(p))
#define	getchar()	getc(stdin)
#define putc(x,p) (--(p)->_cnt>=0? ((int)(*(p)->_ptr++=(unsigned)(x))):_flsbuf((unsigned)(x),p))
#define	putchar(x)	putc(x,stdout)
#define	feof(p)		(((p)->_flag&_IOEOF)!=0)
#define	ferror(p)	(((p)->_flag&_IOERR)!=0)
#define	fileno(p)	((p)->_file)

FILE	*fopen();
FILE	*fdopen();
FILE	*freopen();
long	ftell();
char	*fgets();
#endif