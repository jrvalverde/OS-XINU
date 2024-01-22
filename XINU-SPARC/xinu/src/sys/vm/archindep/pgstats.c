/* 
 * pgstats.c - print virtual memory statistics
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Feb  8 17:28:23 1989
 */


/* pgstats.c - pgstats */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <vmem.h>
#include <proc.h>
#include <pgio.h>


/*------------------------------------------------------------------------
 *  pgstats -	print paging statistics
 *------------------------------------------------------------------------
 */
pgstats(stdin, stdout, stderr, showtimes, sorttimes, toss)
int stdin, stdout, stderr;
int showtimes, sorttimes, toss;
{
    char	sb[128];
    long	i;
    int	sintimes[ITIMSIZE];		/* sorted in times		*/
    int	souttimes[OTIMSIZE];		/* sorted out times		*/
    int	tinaver, tinmed, toutaver, toutmed; /* tossed aver and med */
    int	runsecs;			/* # secs since boot		*/
    int	tpgfaults;			/* total # pg faults		*/
    long	tmptime;		/* temp time variable		*/

	
    sprintf(sb, "Paging Information and Statistics\n\n");
    write(stdout, sb, strlen(sb));
	
    sprintf(sb,
	    "\tDisk Faults:           %8ld    In Memory Faults:         %ld\n",
	    pg.diskfaults, pg.memfaults);
    write(stdout, sb, strlen(sb));
    sprintf(sb,
	    "\tSim Ref Bit Faults:    %8ld    Address Violations:       %ld\n",
	    pg.simfaults, pg.addrviolation);
    write(stdout, sb, strlen(sb));
    sprintf(sb,
	    "\tZero On Demand Faults: %8ld\n", pg.zodfaults);
    write(stdout, sb, strlen(sb));
    sprintf(sb,
	    "\tPages Written To Disk: %8ld\n\n",
	    pg.pagingouts);
    write(stdout, sb, strlen(sb));
	
    sprintf(sb, "\tAccumulated Page Out Time:  %10ld ms\n",
	    accouttime/1000);
    write(stdout, sb, strlen(sb));
    sprintf(sb, "\tAccumulated Page In Time:   %10ld ms\n\n",
	    accintime/1000);
    write(stdout, sb, strlen(sb));

    sprintf(sb, "\tAverage Page Out Time:      %10ld ms\n",
	    avouttime()/1000);
    write(stdout, sb, strlen(sb));
    sprintf(sb, "\tAverage Page In Time:       %10ld ms\n\n",
	    avintime()/1000);
    write(stdout, sb, strlen(sb));

    getrealtim(&tmptime);
    runsecs = tmptime/100;
    if ((tmptime%100) > 49) runsecs++; /* round total runtime	*/
    tpgfaults = pg.diskfaults + pg.memfaults + pg.zodfaults;
    sprintf(sb, "\tAver # Page Faults Per Sec: %4d and (%d/%d)\n",
	    tpgfaults/runsecs, tpgfaults%runsecs, runsecs);
    write(stdout, sb, strlen(sb));
    sprintf(sb, "\tAver # Page-Out Per Sec:    %4d and (%d/%d)\n",
	    pg.pagingouts/runsecs, pg.pagingouts%runsecs, runsecs);
    write(stdout, sb, strlen(sb));
    sprintf(sb, "\tAver # Page-In Per Sec:     %4d and (%d/%d)\n\n",
	    pg.diskfaults/runsecs, pg.diskfaults%runsecs, runsecs);
    write(stdout, sb, strlen(sb));

    timsort(sintimes, souttimes);

    sprintf(sb, "\tMedian Page Out Time:      %10ld ms\n",
	    souttimes[couttimes/2]/1000);
    write(stdout, sb, strlen(sb));
    sprintf(sb, "\tMedian Page In Time:       %10ld ms\n\n",
	    sintimes[cintimes/2]/1000);
    write(stdout, sb, strlen(sb));
	
    if (showtimes && (!sorttimes)) {
	sprintf(sb, "\n\nPage In Times are:\n");
	write(stdout, sb, strlen(sb));
	for (i = 0; i < cintimes; i++) {
	    sprintf(sb, " %ld", intimes[i]/1000);
	    write(stdout, sb, strlen(sb));
	}
	sprintf(sb, "\n\nPage Out Times are:\n");
	write(stdout, sb, strlen(sb));
	for (i = 0; i < couttimes; i++) {
	    sprintf(sb, " %ld", outtimes[i]/1000);
	    write(stdout, sb, strlen(sb));
	}
	sprintf(sb, "\n");
	write(stdout, sb, strlen(sb));
    }
    else if (showtimes) {
	sprintf(sb, "\n\nPage In Times are:\n");
	write(stdout, sb, strlen(sb));
	for (i = 0; i < cintimes; i++) {
	    sprintf(sb, " %ld", sintimes[i]/1000);
	    write(stdout, sb, strlen(sb));
	}
	sprintf(sb, "\n\nPage Out Times are:\n");
	write(stdout, sb, strlen(sb));
	for (i = 0; i < couttimes; i++) {
	    sprintf(sb, " %ld", souttimes[i]/1000);
	    write(stdout, sb, strlen(sb));
	}
	sprintf(sb, "\n");
	write(stdout, sb, strlen(sb));
    }

    if (toss) {
	hilodrop(sintimes, souttimes,
		 &tinaver, &toutaver, &tinmed, &toutmed);

	sprintf(sb, "After tossing High and Low values\n\n");
	write(stdout, sb, strlen(sb));

	sprintf(sb, "\tAverage Page Out Time:      %10ld ms\n",
		toutaver/1000);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "\tAverage Page In Time:       %10ld ms\n\n",
		tinaver/1000);
	write(stdout, sb, strlen(sb));
		
	sprintf(sb, "\tMedian Page Out Time:      %10ld ms\n",
		toutmed/1000);
	write(stdout, sb, strlen(sb));
	sprintf(sb, "\tMedian Page In Time:       %10ld ms\n\n",
		tinmed/1000);
	write(stdout, sb, strlen(sb));
    }
	
    return(OK);
}



/*---------------------------------------------------------------------------
 * timsort - sort the in and out times
 *---------------------------------------------------------------------------
 */
timsort(sintimes, souttimes)
int	sintimes[];	/* sorted in times		*/
int	souttimes[];	/* sorted out times		*/
{
	int i,j, k;
	
	for (i = 0; i < cintimes; i++)
	    sintimes[i] = intimes[i];
	for (i = 0; i < couttimes; i++)
	    souttimes[i] = outtimes[i];
	for (i = 1; i < cintimes; i++) {
		for (j = 0; j < (cintimes-i); j++) {
			if (sintimes[j] > sintimes[j+1]) {
				k = sintimes[j];
				sintimes[j] = sintimes[j+1];
				sintimes[j+1] = k;
			}
		}
	}
	for (i = 1; i < couttimes; i++) {
		for (j = 0; j < (couttimes-i); j++) {
			if (souttimes[j] > souttimes[j+1]) {
				k = souttimes[j];
				souttimes[j] = souttimes[j+1];
				souttimes[j+1] = k;
			}
		}
	}
}


/*---------------------------------------------------------------------------
 * hilodrop - drop the hight and the low values
 *---------------------------------------------------------------------------
 */
hilodrop(sintimes, souttimes, inaver, outaver, inmed, outmed)
int	sintimes[];	/* sorted in times		*/
int	souttimes[];	/* sorted out times		*/
int	*inaver, *outaver, *inmed, *outmed;
{
	int i,j, k;
	long total;
	int nindrop, noutdrop;


	nindrop = cintimes/20;		/* drop 5% of hi and low times	*/
	noutdrop = couttimes/20;	/* drop 5% of hi and low times	*/
	total = 0;
	for (i = 0; i < (cintimes-(2*nindrop)); i++) {
		sintimes[i] = sintimes[i+nindrop];
		total += sintimes[i];
	}
	*inaver = total/i;
	*inmed = sintimes[i/2];
	total = 0;
	for (i = 0; i < (couttimes-(2*noutdrop)); i++) {
		souttimes[i] = souttimes[i+noutdrop];
		total += souttimes[i];
	}
	*outaver = total/i;
	*outmed = souttimes[i/2];
}
