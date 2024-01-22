/* shell.h */

#include <mark.h>

/* Declarations for the Xinu shell */

#define	SHEXIT		-5		/* shell exit return		*/
#define	SHNAMLEN	9		/* length of user name + 1	*/
#define	SHBUFLEN	128		/* length of general buffer	*/
#define	SHARGLEN	130		/* length of area for arguments	*/
#define	SHMAXTOK	18		/* maximum tokens per line	*/
#define	SHMLEN		16		/* maximum length of mach. name	*/
#define	SHCMDSTK	1024		/* size of command proc. stack	*/
#define	SHCMDPRI	20		/* command process priority	*/

struct	shvars	{			/* shell variables		*/
	long	shlast;			/* time shell last touched	*/
	long	shlogon;		/* time user logged on		*/
	int	shncmds;		/* number of commands available	*/
	MARKER	shmark;			/* marked if pused valid	*/
	Bool	shused;			/* TRUE => user using shell	*/
	char	shuser[SHNAMLEN];	/* current user name		*/
	char	shmach[SHMLEN];		/* name of this machine		*/
	char	shbuf[SHBUFLEN];	/* general purpose buffer	*/
	char	*shtok[SHMAXTOK];	/* pointers to input tokens	*/
	char	shtktyp[SHMAXTOK];	/* type of token in shtok[i]	*/
	char	shargst[SHARGLEN];	/* actual strings of arguments	*/
};

extern	struct	shvars	Shl;

#ifndef	NAMESPACE
#define	NAMESPACE	SYSERR
#endif	NAMESPACE

#ifndef	RFILSYS
#define	RFILSYS		SYSERR
#endif	RFILSYS
