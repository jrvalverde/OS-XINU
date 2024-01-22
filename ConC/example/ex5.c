/* ex5.c - two tasks read from stdin, one task writes messages to stdout
 *	   asking for input to be given, sleeps 5 seconds, writes again, etc.
 */

#include <stdio.h>

#define DATA	0
#define CURRENT 1

#define SB_LEN	200

extern int errno;

int	just_read = 0,
	num_alive = 2;

main()
{
    int input(),output(), sh_sem, def = DATA;
    char ch;
    FILE *fopen(), *fd_in, *fd_out;

    if ((fd_in  = fopen("data/ex5.in","r")) == (FILE *)NULL)
	if ((fd_in  = fopen("ex5.in","r")) == (FILE *)NULL) {
	    printf("Sorry.\tInput file cannot be found.\n");
	    printf("\tEither run this from the 'example' directory\n");
	    printf("\tor create a file called 'ex5.in' in which is a\n");
	    printf("\tlist of comments (one per line) that ask for input.\n");
	    exit(1);
	    }
	else {
	    def = CURRENT;
	    if ((fd_out = fopen("ex5.out","w")) == (FILE *)NULL) {
		printf("Sorry.\tOutput file cannot be created.\n\t");
		printf("User must not have write access to this directory.\n");
		exit(1);
		}
	    }
    else
	if ((fd_out = fopen("data/ex5.out","w")) == (FILE *)NULL) {
	    printf("Sorry.\tOutput file cannot be created.\n");
	    printf("\tUser must not have write access to 'data' directory.\n");
	    exit(1);
	    }

    printf("\nThis program has three tasks running:\n");
    fprintf(fd_out,"\nThis program has three tasks running:\n");
    printf("\t2 reading from input and\n");
    fprintf(fd_out,"\t2 reading from input and\n");
    printf("\t1 reading from a file, printing to the terminal,\n");
    fprintf(fd_out,"\t1 reading from a file, printing to the terminal,\n");
    printf("\t\tthen sleeping for 5 seconds.\n\n");
    fprintf(fd_out,"\t\tthen sleeping for 5 seconds.\n\n");
    printf("All output will be echoed to file 'ex5.out' in %s directory\n",
					def == DATA ? "'data'" : "current");
    fprintf(fd_out,"All output will be echoed to file 'ex5.out' in %s directory\n",
					def == DATA ? "'data'" : "current");
    printf("\tto test writing to a file.\n\n");
    fprintf(fd_out,"\tto test writing to a file.\n\n");
    printf("This shows how 2 processes are queued up to read from the same\n");
    fprintf(fd_out,"This shows how 2 processes are queued up to read from the same\n");
    printf("\tinput and how this reading does not block other task(s).\n\n");
    fprintf(fd_out,"\tinput and how this reading does not block other task(s).\n\n");
    printf("To get the full effect, wait a long time (about 30 seconds)\n");
    fprintf(fd_out,"To get the full effect, wait a long time (about 30 seconds)\n");
    printf("\tbefore typing anything in.  When you want to stop, type\n");
    fprintf(fd_out,"\tbefore typing anything in.  When you want to stop, type\n");
    printf("\t'quit' twice (once for each input task).\n\n");
    fprintf(fd_out,"\t'quit' twice (once for each input task).\n\n");
    write (STDOUT,"Hit return when you want to continue:",37);
    fprintf(fd_out,"Hit return when you want to continue:\n");

    read(STDIN,&ch,1);
    while(ch != '\n')
	read(STDIN,&ch,1);

    write(fd_out,"\n",1);

    sh_sem = screate(1);
		
    tstart(input,3,1,fd_out,sh_sem);
    tstart(input,3,2,fd_out,sh_sem);
    tstart(output,3,fd_in,fd_out,sh_sem);

}

input(i,fd_out,sem)
int  i, sem;
FILE *fd_out;
{
    char	sb[SB_LEN];

    gets(sb);
    while(strcmp(sb,"quit") != 0) {
	swait(sem);
	    just_read++;
	ssignal(sem);
	printf("input process %d ==>\t%s\n\n",i,sb);
	fprintf(fd_out,"input process %d ==>\t%s\n\n",i,sb);

	gets(sb);
	}

    swait(sem);
	just_read++;
	num_alive--;
    ssignal(sem);

    printf("input process %d ==>\t%s\n\n",i,sb);
    fprintf(fd_out,"input process %d ==>\t%s\n\n",i,sb);

}

output(fd_in,fd_out,sem)
FILE *fd_in,*fd_out;
int  sem;
{
    char sb[SB_LEN];

    while(num_alive > 0) {			/* reads shared data (not    */
	while(just_read == 0) {			/* concerned with race cond) */
	    if (fgets(sb,SB_LEN,fd_in) == NULL) {
		rewind(fd_in);
		fgets(sb,SB_LEN,fd_in);
		}

	    printf("%s\n",sb);
	    fprintf(fd_out,"%s\n",sb);

	    sleep(5);
	    }
	swait(sem);
	    just_read = 0;
	ssignal(sem);

	printf("Finally you enterred something!!  Do it again.\n\n");
	fprintf(fd_out,"Finally you enterred something!!  Do it again.\n\n");

	rewind(fd_in);
	}

    printf("\n\nThank you for playing this game.\n");
    fprintf(fd_out,"\n\nThank you for playing this game.\n");

    fclose(fd_in);
    fclose(fd_out);
}
