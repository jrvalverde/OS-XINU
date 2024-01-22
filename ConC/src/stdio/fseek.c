/****************************************/
/*					*/
/*	Modified from original source:	*/
/*					*/
/*	swait, ssignal added		*/
/*					*/
/****************************************/

/* @(#)fseek.c	4.3 (Berkeley) 9/25/83 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fseek.c	5.3 (Berkeley) 3/9/86";
#endif LIBC_SCCS and not lint

/*
 * Seek for standard library.  Coordinates with buffering.
 */

#include	<stdio.h>

long lseek();

fseek(iop, offset, ptrname)
	register FILE *iop;
	long offset;
{
	register resync, c;
	long p = -1;			/* can't happen? */
    long ret_val;

    stdio_swait(iop);

	iop->_flag &= ~_IOEOF;
	if (iop->_flag&_IOREAD) {
		if (ptrname<2 && iop->_base &&
			!(iop->_flag&_IONBF)) {
			c = iop->_cnt;
			p = offset;
			if (ptrname==0) {
				long curpos = lseek(stdio_fileno(iop), 0L, 1);
				if (curpos == -1)
					{stdio_ssignal(iop); return (-1);}
				p += c - curpos;
			} else
				offset -= c;
			if(!(iop->_flag&_IORW) && c>0&&p<=c
			    && p>=iop->_base-iop->_ptr){
				iop->_ptr += (int)p;
				iop->_cnt -= (int)p;
				stdio_ssignal(iop);
				return(0);
			}
			resync = offset&01;
		} else 
			resync = 0;
		if (iop->_flag & _IORW) {
			iop->_ptr = iop->_base;
			iop->_flag &= ~_IOREAD;
			resync = 0;
		}
		p = lseek(stdio_fileno(iop), offset-resync, ptrname);
		iop->_cnt = 0;
		if (resync && p != -1)
			if (stdio_getc(iop) == EOF)
				p = -1;
	}
	else if (iop->_flag & (_IOWRT|_IORW)) {
		p = stdio_fflush(iop);
		if (iop->_flag & _IORW) {
			iop->_cnt = 0;
			iop->_flag &= ~_IOWRT;
			iop->_ptr = iop->_base;
		}
		ret_val = (lseek(stdio_fileno(iop), offset, ptrname) == -1 || p == EOF ?
		    -1 : 0);
		stdio_ssignal(iop); return(ret_val);
	}

    stdio_ssignal(iop);

	return(p==-1?-1:0);
}
