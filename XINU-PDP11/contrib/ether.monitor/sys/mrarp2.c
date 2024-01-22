/* mrarp.c - mrarp */

#include <conf.h>
#include <kernel.h>
#include <deqna.h>
#include <ether.h>
#include "../h/ip.h"
#include <arp.h>
#include <ctype.h>
#include "../h/network.h"
#include "../h/monitor.h"

extern	struct	epacket	*epptr;
extern	Bool	match;
extern	Bool	found;

/*------------------------------------------------------
 * mrarp - print RARP packet information
 *------------------------------------------------------
 */

mrarp(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	i, j, length;
	char	str[80];
	IPaddr	faddr, taddr;
	struct	arppak	*arpptr;
	struct	name	*addptr;

	fprintf(0,"ptype=%04x\n",net2hs(epptr->e_ptype));
	switch(net2hs(epptr->e_ptype)){
		case EP_RARP:
			break;
		default:
			userret();
	}
	length = 28;
	arpptr = (struct arppak*)epptr->ep_data;
	for (i=1; i<nargs; i++){
		switch(args[i][0]){
			case 'F':
				args[i] += 1;
				if((addptr=lookup(table,args[i])) == SYSERR){
					fprintf(stderr,"Invalid address\n");
					found = FALSE;
					userret();
				}
				args[i] = addptr->n_ipaddr;
				write(stdout,str,strlen(str));
				args[i] -= 1;
			case 'f':
				args[i] += 1;
				if(ipparse(faddr,args[i]) == SYSERR){
					fprintf(stderr,"Invalid address\n");
					found = FALSE;
					userret();
				}
				fprintf(0,"faddr=%d.%d.%d.%d\n",faddr[0]&0377,faddr[1]&0377,faddr[2]&0377,faddr[3]&0377);
				fprintf(0,"spa=%d.%d.%d.%d\n",arpptr->ar_spa[0]&0377,arpptr->ar_spa[1]&0377,arpptr->ar_spa[2]&0377,arpptr->ar_spa[3]&0377);
				for(j=0; j<4; j++){
					if(faddr[j] != arpptr->ar_spa[j]){
						userret();
					}
				}
				break;

			case 'T':
				args[i] += 1;
				if((addptr=lookup(table,args[i])) == SYSERR){
					fprintf(stderr,"Invalid address\n");
					found = FALSE;
					userret();
				}
				args[i] = addptr->n_ipaddr;
				args[i] -= 1;
			case 't':
				args[i] += 1;
				if(ipparse(taddr,args[i]) == SYSERR){
					fprintf(stderr,"Invalid address\n");
					found = FALSE;
					userret();
				}
				for(j=0; j<4; j++){
					if(taddr[j] != arpptr->ar_tpa[j]){
						userret();
					}
				}
				break;


			default:
				fprintf(stderr,"Syntax error\n");
				found = FALSE;
				userret();
		}
	}
	match = TRUE;
	addptr = checknum(arp_tab, net2hs(arpptr->ar_op));
	sprintf(str,"hrd=%4d, prot=%04x, hlen=%d, plen=%d, op=%7s\n",net2hs(arpptr->ar_hrd),net2hs(arpptr->ar_prot),arpptr->ar_hlen,arpptr->ar_plen,addptr->s_lname);
	write(stdout, str, strlen(str));
	sprintf(str,"sha=%02x%02x.%02x%02x.%02x%02x,",arpptr->ar_sha[0]&0377,arpptr->ar_sha[1]&0377,arpptr->ar_sha[2]&0377,arpptr->ar_sha[3]&0377,arpptr->ar_sha[4]&0377,arpptr->ar_sha[5]&0377);
	sprintf(&str[strlen(str)]," tha=%02x%02x.%02x%02x.%02x%02x\n",arpptr->ar_tha[0]&0377,arpptr->ar_tha[1]&0377,arpptr->ar_tha[2]&0377,arpptr->ar_tha[3]&0377,arpptr->ar_tha[4]&0377,arpptr->ar_tha[5]&0377);
	write(stdout, str, strlen(str));
	sprintf(str,"spa=%d.%d.%d.%d, tpa=%d.%d.%d.%d\n",arpptr->ar_spa[0]&0377,arpptr->ar_spa[1]&0377,arpptr->ar_spa[2]&0377,arpptr->ar_spa[3]&0377,arpptr->ar_tpa[0]&0377,arpptr->ar_tpa[1]&0377,arpptr->ar_tpa[2]&0377,arpptr->ar_tpa[3]&0377);
	write(stdout, str, strlen(str));

	return(OK);
}
