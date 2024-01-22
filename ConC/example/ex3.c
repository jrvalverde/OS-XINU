/* ex3.c - start 3 tasks, each one printing out a differect character */

#include <kernel.h>

int shared = 0;

main()
{
    int pr(), sh_sem;

    sh_sem = screate(1);

    tstart(pr,2,'A',sh_sem);		/* nice interface to		*/
					/* get a task running		*/

    resume(create(pr,INITSTK,getprio(gettid()),START_NAME,2,'B',sh_sem));
					/* what tstart actually does	*/

    resume(create(pr,2000,INITPRIO,"pr C",2,'C',sh_sem));
					/* you can put your own values	*/
					/* for arguments (priority is	*/
					/* INITPRIO here because that is*/
					/* what main starts with, thus	*/
					/* also what the first two tasks*/
					/* start with)			*/
}

pr(ch,sem)
char ch;
int sem;
{
    int i,j;

    for ( i = 0 ; i < 150 ; i++) {
	for ( j = 0 ; j < 5000 ; j++)
	    /* NULL -- TIMER */;
	swait(sem);
	putchar(ch);
	if (++shared >= 30) {
	    putchar('\n');
	    shared = 0;
	    }
	ssignal(sem);
	}
}
