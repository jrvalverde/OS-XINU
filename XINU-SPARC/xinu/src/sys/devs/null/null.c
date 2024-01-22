/* null.c - routines to implement the null device */

#include <conf.h>
#include <kernel.h>
#include <tty.h>

nullopen(devptr,buf,len)
     struct	devsw	*devptr;
     char *buf;
     int len;
{
	return(devptr->dvnum);
}

nullread(devptr, buff, count)
     struct	devsw	*devptr;
     int count;
     char *buff;
{
	return(EOF);
}

nullwrite(devptr, buff, count)
     struct	devsw	*devptr;
     int count;
     char *buff;
{
	return(count);
}

nullcntl(devptr, func, arg, arg2)
     struct devsw	*devptr;
     int func;
     char *arg, *arg2;
{
	switch (func) {
	      case SETNBIO:
	      case UNSETNBIO:
	      case UNNOTIFYR:
	      case UNNOTIFYW:
		return(OK);

	      case NOTIFYW:
	      case NOTIFYR:
		return(IOREADY);

	      default:
		return(OK);
	}
}


nullgetc()
{
	return(EOF);
}
