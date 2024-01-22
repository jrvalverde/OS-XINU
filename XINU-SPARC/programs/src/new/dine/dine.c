/* 
 * dine.c - Dining Philosophers Problem for VM Xinu
 * 
 * Author:	Victor Norman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jul 18 10:42:56 1990
 *
 * Copyright (c) 1990 Victor Norman
 */

#define NumPhils 5		/* number of philosophers */

int sem[NumPhils];		/* each semaphore represents a chopstick */

main(argc, argv)
int argc;
char *argv[];
{
	int i,j;
	int pid[NumPhils];
	int loop();
	char str[40];

	/* dynamically create the semaphores */
	for (i = 0; i < NumPhils; i++) {
		j = xscreate(1);
		sem[i] = j;
	}

	/* create a process for each philosopher */
	for (i = 0; i < NumPhils; i++) {
	    sprintf(str, "phil%d", i);
	    printf("started %s\n", str);
	    pid[i] = xtcreate(loop, 20, str, 1, i);
	}

	/* start all the philosophers' processes */
	for (i = 0; i < NumPhils; i++)
	    xresume(pid[i]);

}	

/*
 * loop -- each philosopher runs this loop.
 */
loop(i)
int i;
{
    while(1) {
	    /* think for a small random length of time */
	    printf("Philosopher %d Thinking\n", i);
	    xsleep(rand() & 7);

	    /*
	     * go get the chopsticks -- odd numbered philosophers go
	     * for the left chopstick first, then the right.  Even
	     * numbered philosophers go for the right chopstick first,
	     * then the left.  This ensures that no philosopher will
	     * starve and no deadlock will occur.
	     */
	    
	    if (i % 2) {		/* odd numbered philosopher */
	            xwait(sem[i]);
	            xwait(sem[(i + 1) % NumPhils]);
	    }
	    else {			/* even numbered philosopher */
		    xwait(sem[(i + 1) % NumPhils]);
		    xwait(sem[i]);
	    }

	    /* eat for a small random length of time */
	    printf("Philosopher %d Eating\n", i);
	    xsleep(rand() & 7);

	    /* release the chopsticks -- order doesn't matter */
	    xsignal(sem[i]);
	    xsignal(sem[(i + 1) % NumPhils]);
    }
}

