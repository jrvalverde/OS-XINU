/* io.h -  fgetc, fputc, getchar, isbaddev, putchar */

#include <iodisp.h>

#if defined(NDEVS) && NDEVS > 0
#define	INTVECI	((int)devveci0)	/* lowest input interrupt dispatch loc	*/
#define	INTVECO	((int)devveco0)	/* lowest output interrupt dispatch loc	*/
#define DVECSIZE (INTVECO-INTVECI)/* bytes per dispatch vector-to loc 	*/
#else
#define INTVECI NULLPTR		/* dummy definitions 			*/
#define INTVECO NULLPTR
#define DVECSIZE 0
#endif

struct	intmap	{		/* device-to-interrupt routine mapping	*/
	int	(*iin)();	/* address of input interrupt routine	*/
	int	icode;		/* argument passed to input routine	*/
	int	(*iout)();	/* address of output interrupt routine	*/
	int	ocode;		/* argument passed to output routine	*/
	};

#ifdef	NDEVS
extern	struct	intmap intmap[NDEVS];
#define	isbaddev(f)	( (f)<0 || (f)>=NDEVS )
#endif

#define	BADDEV		-1

/* In-line I/O procedures */

#define	getchar()	getc(CONSOLE)
#define	putchar(ch)	putc(CONSOLE,(ch))
#define	fgetc(unit)	getc((unit))
#define	fputc(unit,ch)	putc((unit),(ch))
