/* ex4.c - Dining Philosophers problem. */

#define BIG_NO		2147483647
#define MAX_SLEEP	10
#define random()	((rand() / (BIG_NO/MAX_SLEEP)) + 1)


int	eat_sem,
	fork_sem[5];

main()
{
    int i, philo();

    eat_sem = screate(4);		/* allow only 4 to eat at a time*/
					/* to prevent deadlock		*/

    for ( i = 0 ; i <= 4 ; i++ )
	fork_sem[i] = screate(1);	/* create fork semaphores	*/

    tstart(philo,1,0);			/* get philosophers running	*/
    tstart(philo,1,1);
    tstart(philo,1,2);
    tstart(philo,1,3);
    tstart(philo,1,4);
}

philo(me)
int me;
{
    int times, time;

    for ( times = 0 ; times <= 4 ; times++ ) {
	printf("\tP%d meditating for %d sec.\n",me,time = random());
	sleep(time);
	printf("\tP%d finish meditating.  Waiting for table.\n",me);

	swait(eat_sem);				/* wait for spot at table   */
	    printf("\t\tP%d at table.  Waiting for left fork.\n",me);

	    swait(fork_sem[(me + 1) % 5]);	/* wait for left  fork	    */
		printf("\t\t\tP%d got left fork.  Waiting for right.\n",
									    me);

		swait(fork_sem[me]);		/* wait for right fork	    */

		    printf("\t\t\t\tP%d got right fork and eating for %d sec.\n",
							me,time = random());
		    sleep(time);
		    printf("\t\t\t\tP%d finished eating and relinquishing right fork.\n",me);

		ssignal(fork_sem[me]);		/* relenquishing right fork */
		printf("\t\t\tP%d relenquishing left fork.\n",me);

	    ssignal(fork_sem[(me + 1) % 5]);	/* relenquishing left  fork */
	    printf("\t\tP%d relenquishing place at table.\n",me);

	ssignal(eat_sem);			/* leaving table	    */
	}

    printf("P%d finished.\n",me);
}
