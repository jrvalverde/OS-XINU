/* tcpackit.c - tcpackit */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <nettcp.h>
#include <mark.h>
#include <proc.h>

/*#define SJCDEBUG*/
/*#define LOG_TO_FILE*/
/*#define LOG_TO_CONSOLE*/
#define LOGFILE "/tmp/tcplog"

#define MAXPACKS 50

struct tcp_info_st {
    IPaddr ip_addr;
    unsigned long timestamp;
    unsigned int window;
    unsigned int lport, fport;
    unsigned long sequence, ack;
    unsigned int len;
    unsigned int type;
    unsigned int code;
    unsigned int arg1;
    unsigned int arg2;
    char *sb;
} tcp_info[MAXPACKS+10];

MARKER sjc_mark;
unsigned long first_time;
static int ixtcp;
Bool rec_dologging;
extern u_long clktime;
/* local, static routines */
static rec_islport();
static int rec_tcp_seg_print_lowlevel();


/*------------------------------------------------------------------------
 *  rec_tcp_seg
 *------------------------------------------------------------------------
 */
int rec_tcp_seg(pep, iflag, sb, a1, a2)
     
struct	ep	*pep;
int     iflag;          /* TRUE => input, FALSE => output */
char    *sb;

{
	STATWORD ps;
	struct tcp_info_st *tip;
	struct ip *pip = (struct ip *)pep->ep_data;
	struct tcp *ptcp = (struct tcp *)pip->ip_data;

	if (!rec_dologging)
	    return;

	if (unmarked(sjc_mark)) {
		rec_dologging = FALSE;
/*		rec_dologging = TRUE;*/
		first_time = 0;
		ixtcp = 0;
		mark(sjc_mark);
	}


	if ((pep) && ((pep->ep_type != EPT_IP) ||
		      (pip->ip_proto != IPT_TCP)))
	    return;

	disable(ps);

	tip = &tcp_info[ixtcp++];

	bzero(tip,sizeof(struct tcp_info_st));

	tip->timestamp = clktime;

	if (!first_time)
	    first_time = tip->timestamp;

	tip->sb = sb;
	tip->type = iflag;
	tip->arg1 = a1;
	tip->arg2 = a2;

	if (pep) {
		if (tip->type) {
			/* input half */
			IP_COPYADDR(tip->ip_addr, pip->ip_src);
			tip->lport = ptcp->tcp_dport;
			tip->fport = ptcp->tcp_sport;
		} else {
			/* output half */
			IP_COPYADDR(tip->ip_addr, pip->ip_dst);
			tip->fport = ptcp->tcp_dport;
			tip->lport = ptcp->tcp_sport;
		}

		tip->sequence = ptcp->tcp_seq;
		tip->ack = ptcp->tcp_ack;
		tip->window = ptcp->tcp_window;
		tip->code = ptcp->tcp_code;
		tip->len = pip->ip_len - IP_HLEN(pip) - TCPMHLEN;

		/* byte swap, if necessary */
		if (!rec_islport(tip->lport)) {
			/* byte swap them */
			tip->sequence	= net2hl(tip->sequence);
			tip->ack	= net2hl(tip->ack);
			tip->fport	= net2hs(tip->fport);
			tip->lport	= net2hs(tip->lport);
			tip->window	= net2hs(tip->window);
			tip->len	= net2hs(pip->ip_len) - IP_HLEN(pip) - TCPMHLEN;
		}
	}

	restore(ps);

	if (ixtcp >= MAXPACKS) {
#ifdef LOG_TO_FILE
		rec_tcp_seg_print_file();
#endif
#ifdef LOG_TO_CONSOLE
		rec_tcp_seg_print_console();
#endif
		ixtcp = 0;
	}

}

int
rec_tcp_seg_print_console()
{
    rec_tcp_seg_print_lowlevel(CONSOLE);
}


int
rec_tcp_seg_print_file()
{
	int fd;
	
	if ((fd = open(NAMESPACE, LOGFILE, "wn")) <= 0) {
		kprintf("rec_tcp_seg_print_file: Can't open %s\n", LOGFILE);
		return;
	}
	
	kprintf("rec_tcp_seg_print_file: writing %d recs to dev:%d (%s)\n",
		ixtcp, fd, LOGFILE);

	rec_tcp_seg_print_lowlevel(fd);

	close(fd);
}



static int rec_tcp_seg_print_lowlevel(fd)
     int fd;
{
	int i;
	int j;
	struct tcp_info_st *tip;
	static char outbuf[200];
	static char msgbuf[200];
	int lasttime;

	rec_dologging = FALSE;

#define HEADER1 "         dir   IP            local rem   sequence ack                 code    \n"
#define HEADER2 "time     dir   Addr          port  port  (hex)    (hex)    len  wind  --UAPRSF\n"
#define HEADER3 "======== === =============== ===== ===== ======== ======== ==== ===== ========\n"

	if (fd) {
		write(fd,HEADER1,strlen(HEADER1));
		write(fd,HEADER2,strlen(HEADER2));
		write(fd,HEADER3,strlen(HEADER3));
	} else {
		kprintf(HEADER1);
		kprintf(HEADER2);
		kprintf(HEADER3);
	}

	lasttime = 0;
	for (i=0; i < ixtcp; ++i) {
		tip = &tcp_info[i];

		j = (tip->timestamp - first_time) / 1000;

		if ((j - lasttime) > 5)
		    if (fd)
			write(fd,"\n",1);
		    else
			kprintf("\n");

		if (tip->sb)
		    sprintf(msgbuf,tip->sb,tip->arg1,tip->arg2);
		else
		    *msgbuf='\00';

		sprintf(outbuf,
			"%4d.%03d %3s %3d.%03d.%03d.%03d %5d %5d %08x %08x %4d %5d %08b - %s\n",
			(unsigned int) j / 1000,
			(unsigned int) j % 1000,
			(tip->type) ? "in " : "out",
			(unsigned char) tip->ip_addr[0],
			(unsigned char) tip->ip_addr[1],
			(unsigned char) tip->ip_addr[2],
			(unsigned char) tip->ip_addr[3],
			(unsigned short) tip->lport,
			(unsigned short) tip->fport,
			(unsigned long) tip->sequence,
			(unsigned long) tip->ack,
			(unsigned short) tip->len,
			(unsigned short) tip->window,
			(unsigned char) tip->code,
			msgbuf);
		if (fd)
		    write(fd,outbuf,strlen(outbuf));
		else
		    kprintf(outbuf);
		lasttime = j;
	}

	ixtcp = 0;
	rec_dologging = TRUE;
}


static rec_islport(port)
     unsigned int port;
{
	int		tcbn;

	for (tcbn=0; tcbn<Ntcp; ++tcbn) {
		if (tcbtab[tcbn].tcb_state == TCPS_FREE)
			continue;
		if (port == tcbtab[tcbn].tcb_lport)
		    return(TRUE);
	}
	return(FALSE);
}
