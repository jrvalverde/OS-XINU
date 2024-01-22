/************************************************************************/
/*									*/
/* fsd - set user and group ids, and exec Xinu file server		*/
/*									*/
/*   Note: this program is execute from /etc/rc.local to start the	*/
/*		Xinu file server at system boot.  It changes the	*/
/*		user and group ids of the executing process to that	*/
/*		of user "xinu", making it impossible to access files	*/
/*		remotely that are inaccessible by "xinu" locally.	*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <pwd.h>
#include <grp.h>

/* User and group protection to assume when executing */

#define USERNAME	"xinu"
#define	GROUPNAME	"xinu"
#define WORKINGDIR	"/"
#define FILESERVER	"/usr/xinu7/bin/fs"
char	*fname = FILESERVER;

/*------------------------------------------------------------------------
 *  main  -  set user and group id and exec Xinu file server
 *------------------------------------------------------------------------
 */
main(argc, argv, envp)
int	argc;
char	*argv[];
char	*envp[];
{
	int	olduid = getuid();
	struct	passwd	*pw;
	struct	group	*gr;
	int	i;

	/* Look up group and set executing proceess gid accordingly	*/

	if ((gr=getgrnam(GROUPNAME)) == NULL) {
		fprintf(stderr, "Cannot find group %s\n", GROUPNAME);
		exit(1);
	}
	if (setgid(gr->gr_gid) < 0) {
		fprintf(stderr, "Cannot set gid to %s\n", GROUPNAME);
		exit(1);
    	}

	/* Loop up user and set execting proceess uid accordingly	*/

	if ((pw=getpwnam(USERNAME)) == NULL) {
		fprintf(stderr, "Cannot find user %s\n", USERNAME);
		exit(1);
	}
	if (setuid(pw->pw_uid) < 0) {
		fprintf(stderr, "Cannot set uid to %s\n", USERNAME);
	        exit(1);
	}

	if (chdir(WORKINGDIR) < 0) {
		fprintf(stderr, "Cannot change to directory %s\n",
			WORKINGDIR);
		exit(1);
	}

	for (i=0 ; i<3 ; i++)
		(void) close(i);

	/* execute file server */

	argv[0] = fname;
	execv(FILESERVER, argv, envp);
	exit(2);
}

