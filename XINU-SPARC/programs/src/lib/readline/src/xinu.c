/* 
 * xinu.c - 	make xinu compatable
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun 28 16:23:52 1990
 *
 */

/*#define BOGUS_DIRSTUFF*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/file.h>

#include <../../../../include/tty.h>		/* xinu header file */

#ifdef BOGUS_DIRSTUFF

typedef struct garbage {
	int junk;
} DIR;
struct dirent {
	int junk;
};

/* closedir -  */
closedir(ptr)
     struct DIR *ptr;
{
}

/*  readdir - */
struct dirent *readdir(ptr)
     struct DIR *ptr;
{
	return(NULL);
}

/* opendir -  */
struct DIR *opendir()
{
	return(NULL);
}

/* getdents - get directory entries */
int getdents()
{
	return(0);
}

#endif BOGUS_DIRSTUFF



/* abort - */
abort()
{
	exit(1);
}



/* fcntl - return error status */
fcntl()
{
	return 0;
}

/* xrawmode - set tty line to rawmode */
xcbreakmode()
{
	xcontrol(readdtable(0), TCMODEK);
	xcontrol(readdtable(0), TCNOECHO);
	stdin->_flag = stdin->_flag | _IONBF;
}

/* xrawmodeoff - set tty line to cooked mode */
xcookedmode()
{
	xcontrol(readdtable(0), TCMODEC);
	xcontrol(readdtable(0), TCECHO);
}
