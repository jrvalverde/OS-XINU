/* hgprint - hgprint */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <igmp.h>

static	printone();

/*------------------------------------------------------------------------
 * hgprint - print the Host Group table on descriptor fd
 *------------------------------------------------------------------------
 */
hgprint(fd)
int	fd;
{
	struct hg	*phg;
	int		i;

	for (i=0; i<HG_TSIZE; ++i)
		printone(fd, &hgtable[i]);
	return OK;
}

char	*index();

/*------------------------------------------------------------------------
 * printone - print one entry in the Host Group table
 *------------------------------------------------------------------------
 */
static
printone(fd, phg)
int	fd;
struct	hg	*phg;
{
	char	line[128], *p;
	int	i, inum;

	if (phg->hg_state == HGS_IDLE)
		strcpy(line, "IDLE ");
	else if (phg->hg_state == HGS_DELAYING)
		strcpy(line, "DELAYING ");
	else
		return OK;
	p = index(line, '\0');

	sprintf(p, "%s", ip2dot(phg->hg_ipa));
	p = index(p, '\0');

	sprintf(p, "intf %d refs %d IPttl %d\n", phg->hg_ifnum,
		phg->hg_refs, phg->hg_ttl);
	return write(fd, line, strlen(line));
}
