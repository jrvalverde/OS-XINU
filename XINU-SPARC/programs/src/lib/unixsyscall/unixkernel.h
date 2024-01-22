
/* 
 * unixkernel.h - Structures need to simulate a unix kernel
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Apr 17 14:28:13 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

/*---------------------------------------------------------------------------
 * Machine Dependent constants/structures
 *---------------------------------------------------------------------------
 */
#define	GUESS_CONSOLE_NUM	0


/*---------------------------------------------------------------------------
 * Machine Independent constants/structures
 *---------------------------------------------------------------------------
 */

#define	DEVTBLSIZE	20

#define	MAXENVIRON	64

struct devicetable {		/* per process file descriptor table	*/
	int	inuse;			/* bool: true if entry in use	*/
	int	xinudevnum;		/* corresponding xinu device	*/
	long	filesize;		/* total file size		*/
};

struct devfnametbl {		/* file name associated with each device */
	char	fname[256];		/* name of the file		*/
};


extern struct devicetable dtable[];
extern struct devfnametbl dfnametable[];

extern char libwarning;			/* print library error messages	*/

extern char **environ;			/* the unix environment variable */
