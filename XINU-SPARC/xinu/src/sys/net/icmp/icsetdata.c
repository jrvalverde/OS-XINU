/* icsetdata.c - icsetdata */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/* ECHOMAX must be an even number */
#define	ECHOMAX(pip)	(MAXLRGBUF-IC_HLEN-IP_HLEN(pip)-EP_HLEN-EP_CRC)

/*------------------------------------------------------------------------
 *  icsetdata -  set the data section. Return value is data length
 *------------------------------------------------------------------------
 */
int icsetdata(type, pip, pa2)
int		type;
struct	ip	*pip;
char		*pa2;
{
	struct	icmp	*pic = (struct icmp *)pip->ip_data;
	int		i, len;

	switch (type) {
	case ICT_ECHORP:
		len = pip->ip_len - IP_HLEN(pip) - IC_HLEN;
		if (isodd(len))
			pic->ic_data[len] = 0;	/* so cksum works */
		return len;
	case ICT_DESTUR:
	case ICT_SRCQ:
	case ICT_TIMEX:
		pic->ic_mbz = 0;		/* must be 0 */
		break;
	case ICT_REDIRECT:
		IP_COPYADDR(pic->ic_gw, pa2);
		break;
	case ICT_PARAMP:
		pic->ic_ptr = (char) pa2;
		for (i=0; i<IC_PADLEN; ++i)
			pic->ic_pad[i] = 0;
		break;
	case ICT_MASKRP:
		IP_COPYADDR(pic->ic_data, pa2);
		break;
	case ICT_ECHORQ:
		if (pa2 > (char *) ECHOMAX(pip))
			pa2 = (char *) ECHOMAX(pip);
		for (i=0; i<(int)pa2; ++i)
			pic->ic_data[i] = i;
		if (isodd(pa2))
			pic->ic_data[(int)pa2] = 0;
		return (int)pa2;
	case ICT_MASKRQ:
		IP_COPYADDR(pic->ic_data, ip_anyaddr);
		return IP_ALEN;
	}
	return 0;
}
