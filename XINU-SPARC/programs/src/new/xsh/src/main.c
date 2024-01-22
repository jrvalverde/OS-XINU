/* 
 * main.c - main program
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 *
 */

#define DEBUG

#include <stdio.h>
#include <sys/file.h>

#include "xsys.h"

#include "main.h"
#include "utils.h"
#include "hash.h"
#include "parse.h"

extern int libwarning;

int 	linecnt = 0;
int	shellpid;
char	pidbuff[8];
int 	xsh_debug = FALSE;

struct cmd_rec *g_cmd = NULL;

#define xsh_usage()	ReportError("usage:", "xsh [-t terminal_type]", NULL)

/*
 * ====================================================================
 * main - main routine
 * ====================================================================
 */
main(argc, argv)
int argc;
char *argv[];
{
	char	str[256];
	char	*line;

	libwarning = 0;
	
	transenv();		/* must be first thing you do */

	setarguments(argc, argv);

	StartUp();
	MakeCmd(argc, argv);
	
	sprintf(str, "%s/%s", getenv("HOME"), RESOURCEFILE);
	if (access(str, R_OK | F_OK) != -1) {
		
#ifdef DEBUG
		fprintf(stdout, "sourcing file %s\n", str);
#endif
		
	    	Source(str);
	}
	
	doit(stdin);
	fprintf(stdout, "\n");
	exit(0);
}


/*
 * ====================================================================
 * StartUp - called upon startup of the shell -- set path, term, user
 * from the environment, set the prompt, etc.
 * ====================================================================
 */
static StartUp()
{
	char	*val, *homedir;
	char	buff[256];
	char	*strsave();
	IPaddr	myaddr;
	char	name[MAX_NAMELEN], *ptr;

	xcontrol(readdtable(0), TCNOINT, xgetpid());
			
#ifdef DEBUG
		fprintf(stdout, "Startup....\n");
#endif
		
	v_inittbl();
	a_inittbl();	

	if ( v_find("prompt") == NULL )
	    v_insert("prompt", DEFAULTPROMPT);
	if ((val = getenv("PATH")) != NULL)
	    v_insert("path", val);
	if ((val = getenv("TERM")) != NULL)
	    v_insert("term", val);
	if ((val = getenv("USER")) != NULL)
	    v_insert("user", val);
	
	if ( getenv("HOME") == NULL ) {
		if ((homedir = getenv("HOMEDIR")) == NULL ) {
			strcpy(buff, DEFAULTDIR);
			if ( val == NULL )
			    strcat(buff, "unknown");
			else
			    strcat(buff, val);
			homedir = buff;
		}
		putenv(MakeEnvString("HOME", homedir));
		putenv(MakeEnvString("PWD", homedir));
	}
	
	shellpid = getpid();
	sprintf(pidbuff, "%d", shellpid);

	if ( getenv("HOST") == NULL ) {
		if ( xgetaddr(myaddr) == SYSERR ) {
			putenv(MakeEnvString("HOST", "NO_ADDR"));
			putenv(MakeEnvString("SHORTHOST", "XSH"));
			return;
		}
		if ( xip2name(myaddr, name) == SYSERR ) {
			putenv(MakeEnvString("HOST", "NO_NAME"));
			putenv(MakeEnvString("SHORTHOST", "XSH"));
			return;
		}
		putenv(MakeEnvString("HOST", name));
		if ( (ptr = index(name, '.')) != NULL ) {
			*ptr = '\0';
			putenv(MakeEnvString("SHORTHOST", name));
		}
	}
}

#define MAXLOGIN 64

/*
 * ====================================================================
 * setarguments - set the arguments
 * ====================================================================
 */
static setarguments(argc, argv)
int argc;
char *argv[];
{
	int i, len;
	char *ptr;
	char p[MAXLOGIN+1];
	int prompt = FALSE;
	
	for ( i = 1; i < argc; i++ ) {
		if ( strequ("-p", argv[i]) ) {
			prompt = TRUE;
		}
		else
		if ( !strncmp("-t", argv[i],2) ) {
			if ( argv[i][2] != '\0' ) {
				ptr = argv[i] + 2;
			}
			else
			if ( (++i) >= argc ) {
				xsh_usage();
				break;
			}
			else
			    ptr = argv[i];
			putenv(MakeEnvString("TERM", ptr));
			v_insert("term", ptr);
			UnsetEnv("TERMCAP");
		}
		else if ( !strncmp("-n", argv[i], 2) ) {
			if ( argv[i][2] != '\0' ) {
				ptr = argv[i] + 2;
			}
			else
			if ( (++i) >= argc ) {
				xsh_usage();
				break;
			}
			else
			    ptr = argv[i];
			putenv(MakeEnvString("USER", argv[i]));
			UnsetEnv("HOME");
		}
	}
	if ( prompt ) {
		len = 0;
		while ( len == 0 ) {
			fprintf(stdout, "login: ");
			if ( (len = read(STDIN, p , MAXLOGIN)) <= 0 ) {
				fprintf(stdout, "\n");
fprintf(stdout, "len = %d, exit\n", len);
				exit(0);
			}
			if ( p[len-1] == '\n' ) 
			    len--;
		    	p[len] = '\0';
		}
fprintf(stdout, "login = [%s]\n", p);
		putenv(MakeEnvString("USER", p));
		UnsetEnv("HOME");
	}
}
   

/*
 * MakeCmd - make the global cmd record.  Used for parameter
 * 	     substitution.
 */
static MakeCmd(argc, argv)
int argc;
char *argv[];
{
	int i;
	struct arglist *args;
	
	g_cmd = NULL;
	
	if ( argc <= 0 )
	    return;
	
	g_cmd = (struct cmd_rec *) xmalloc(sizeof(struct cmd_rec));
	g_cmd->cmd.value = strsave(argv[0]);
	g_cmd->input.value = NULL;
	g_cmd->output.value = NULL;
	g_cmd->backgrounding = FALSE;
	g_cmd->args = NULL;
	g_cmd->next = NULL;

	if ( argc <= 1 )
	    return;

	args=g_cmd->args = (struct arglist *) xmalloc(sizeof(struct arglist));
	args->arg = strsave(argv[1]);
	args->next = NULL;
	
	for ( i = 2; i < argc ; i++) {
		args->next=(struct arglist *) xmalloc(sizeof(struct arglist));
		args = args->next;
		args->arg = strsave(argv[i]);
		args->next = NULL;
	}
}

