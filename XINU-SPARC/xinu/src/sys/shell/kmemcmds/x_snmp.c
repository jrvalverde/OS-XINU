/* x_snmp.c - x_snmp */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <tty.h>
#include <ctype.h>
#include <snmp.h>
#include <asn1.h>
#include <mib.h>

#define EXIT		'\004'		/* \004 is Cntl-d		*/
#define COMPLETE	'\011'		/* \011 is TAB			*/
#define KILLWORD	'\027'		/* \027 is Cntl-w		*/

#define M_COMPL		1		/* completion mode 		*/
#define M_CHARIN	2		/* char-in mode 		*/

#define PROMPT		"snmp > "
#define EXITSTR		"exit"

#define BUFSIZE		256
char	buf[BUFSIZE];			/* buffer to hold the input line*/
char	str2compl[BUFSIZE];		/* string to complete 		*/
int	s2clen;				/* length of str2compl		*/
int	completions[BUFSIZE];		/* array of indices into the mib*/
					/* array to signal completions	*/
int	num_completions;		/* # of indices in completions[]*/
int	next_completion;		/* next completion to try	*/

/*------------------------------------------------------------------------
 * x_snmp - SNMP shell that does MIB object name completion
 *------------------------------------------------------------------------
 */
x_snmp(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	ch, snmpservstr[BUFSIZE];
	struct	tty	*ttyp;
	int	ct, i, mode;
	IPaddr	destmach;

	if (nargs != 2) {
		snusage(stdout);
		return OK;
	}
	args++;  nargs--;
	sninit();
	if (name2ip(destmach, *args) == SYSERR) {
		fprintf(stdout,"snmp: couldn't resolve name %s\n", *args);
		return OK;
	}
	ip2dot(snmpservstr, destmach);
	sprintf(snmpservstr + strlen(snmpservstr), ":%d", SNMPPORT);
	ttyp = &tty[stdin];
	ct = 0;
	mode = M_CHARIN;
	next_completion = num_completions = 0;

	control(stdin, TCMODER);	/* put stdin into raw mode */
	write(stdout, PROMPT, strlen(PROMPT));	/* print the prompt */

	while (TRUE) {
		if ((ch = getc(stdin)) == EOF) {
			write(stdout, EXITSTR, strlen(EXITSTR));
			putc(stdout, '\n');
			control(stdin, TCMODEC);
			return OK;
		}
		if (ch == COMPLETE) {
			if (mode == M_CHARIN) {
				mode = M_COMPL;
				/* find beginning of word */
				for (i=ct-1; i >= 0 && buf[i] != BLANK;
						i--)
					/* empty */;
				s2clen = ct - ++i;
				strncpy(str2compl, buf + i, s2clen);
				find_completions();
			}
			if (num_completions == 0) {
				putc(stdout, BELL);
				mode = M_CHARIN;
			} else
				print_completion(ttyp, &ct, stdout);
			continue;
		}
		if (ch == KILLWORD && mode == M_COMPL) {
			/* kill word in compl. mode goes back to
			   original string to complete. */
			eraseword(ttyp, &ct, stdout);
			strncpy(buf + ct, str2compl, s2clen);
			write(stdout, buf + ct, s2clen);
			ct += s2clen;
			mode = M_CHARIN;
			next_completion = num_completions = 0;
			continue;
		}
		if (mode == M_COMPL) {	/* && ch != KILLWORD */
			mode = M_CHARIN;
			str2compl[(s2clen = 0)] = '\0';
			num_completions = next_completion = 0;
		}
		if (ch == KILLWORD) {	/* && mode != M_COMPL */
			eraseword(ttyp, &ct, stdout);
			continue;
		}
		if (ch == ttyp->ikillc && ttyp->ikill) {
			eraseall(ttyp, ct, stdout);
			ct = 0;
			continue;
		}
		if (ch == ttyp->ierasec	&& ttyp->ierase) {
			if (ct > 0) 
				erase1(ttyp, --ct, stdout);
			continue;
		}
		if (ch == RETURN) {
			echoch(ch, ttyp, stdout);
			buf[ct] = '\0';
			if (strequ(buf, EXITSTR)) {
				control(stdin, TCMODEC);
				return OK;
			}
			sendquery(stdout, snmpservstr);
			for (i = 0; i < BUFSIZE; i++)
				buf[i] = '\0';
			write(stdout, PROMPT, strlen(PROMPT));
			ct = 0;
			continue;
		}
		/* all non-special characters */
		if (ct == BUFSIZE - 1) 
			putc(stdout, BELL);
		else {
			echoch(ch, ttyp, stdout);
			buf[ct++] = ch;
		}
	}
}

/*------------------------------------------------------------------------
 * print_completion - write the next name completion to stdout
 *------------------------------------------------------------------------
 */
print_completion(ttyp, ct, stdout)
int	*ct;
int	stdout;
{
	if (next_completion >= num_completions) {
		putc(stdout, BELL);
		return;
	}
	eraseword(ttyp, ct, stdout);
	strcpy(buf + *ct, mib[completions[next_completion]].mi_prefix);
	strcat(buf + *ct, mib[completions[next_completion++]].mi_name);
	write(stdout, buf + *ct, strlen(buf) - *ct);
	*ct = strlen(buf);
}

/*------------------------------------------------------------------------
 * find_completions - find all the completions in the mib for string
 * 		      str2compl.  Put the indices of the completions
 * 		      into the completions array.
 *------------------------------------------------------------------------
 */
find_completions()
{
	int 	i;
	
	for (i = 0; i < mib_entries; i++) 
		if (strnequ(mib[i].mi_name, str2compl, s2clen))
			completions[num_completions++] = i;
	next_completion = 0;
}

/*------------------------------------------------------------------------
 * parsevalue - parse the type and value of variable to set
 *------------------------------------------------------------------------
 */
parsevalue(word, bl)
char		**word;
struct snbentry	*bl;
{
	if (strequ(*word, "int"))
		SVTYPE(bl) = ASN1_INT;
	else if (strequ(*word, "counter")) 
		SVTYPE(bl) = ASN1_COUNTER;
	else if (strequ(*word, "gauge")) 
		SVTYPE(bl) = ASN1_GAUGE;
	else if (strequ(*word, "timeticks")) 
		SVTYPE(bl) = ASN1_TIMETICKS;
	else if (strequ(*word, "str")) 
		SVTYPE(bl) = ASN1_OCTSTR;
	else if (strequ(*word, "objid")) 
		SVTYPE(bl) = ASN1_OBJID;
	else if (strequ(*word, "ipaddr")) 
		SVTYPE(bl) = ASN1_IPADDR;
	else
		return SYSERR;

	getword(word);
	if (**word == NULLCH)
		return SYSERR;

	switch (SVTYPE(bl)) {
	case ASN1_INT:
	case ASN1_COUNTER:
	case ASN1_GAUGE:
	case ASN1_TIMETICKS:
		SVINT(bl) = atoi(*word);
		return OK;
	case ASN1_OCTSTR:
		SVSTRLEN(bl) = strlen(*word);
		SVSTR(bl) = (char *) getmem(SVSTRLEN(bl));
		blkcopy(SVSTR(bl), *word, SVSTRLEN(bl));
		return OK;
	case ASN1_OBJID:
		SVOIDLEN(bl) = dot2oid(SVOID(bl), *word);
		return OK;
	case ASN1_IPADDR:
		dot2ip(SVIPADDR(bl), *word);
		return OK;
	}
	return OK;
}

/*------------------------------------------------------------------------
 *  echoch  --  echo a character with visual and ocrlf options
 *------------------------------------------------------------------------
 */
LOCAL echoch(ch, ttyp, stdout)
char		ch;
struct tty	*ttyp;
int		stdout;
{
	if ((ch == NEWLINE || ch == RETURN) && ttyp->ecrlf) {
		putc(stdout, RETURN);
		putc(stdout, NEWLINE);
	} else if ((ch < BLANK || ch == 0177) && ttyp->evis) {
		putc(stdout, UPARROW);
		putc(stdout, ch + 0100);	/* make it printable	*/
	} else
		putc(stdout, ch);
}

/*------------------------------------------------------------------------
 *  erase1  --  erase one character honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL erase1(ttyp, ct, stdout)
struct tty	*ttyp;
int 		ct;
int		stdout;
{
	char	ch;

	ch = buf[ct];
	if (ch < BLANK || ch == 0177) {
		if (ttyp->evis)	{
			putc(stdout, BACKSP);
			if (ttyp->ieback) {
				putc(stdout, BLANK);
				putc(stdout, BACKSP);
			}
		}
		putc(stdout, BACKSP);
		if (ttyp->ieback) {
			putc(stdout, BLANK);
			putc(stdout, BACKSP);
		}
	} else {
		putc(stdout, BACKSP);
		if (ttyp->ieback) {
			putc(stdout, BLANK);
			putc(stdout, BACKSP);
		}
	}
}

/*------------------------------------------------------------------------
 *  eraseall  --  erase all characters honoring erasing backspace
 *------------------------------------------------------------------------
 */
LOCAL eraseall(ttyp, ct, stdout)
struct tty	*ttyp;
int		ct;
int		stdout;
{
	for (--ct; ct >= 0; ct--) 
		erase1(ttyp, ct, stdout);
}

/*------------------------------------------------------------------------
 *  eraseword  --  erase the previous word
 *------------------------------------------------------------------------
 */
LOCAL eraseword(ttyp, ct, stdout)
struct tty	*ttyp;
int		*ct;
int		stdout;
{
	/* if first character behind cursor is a blank, eat it up */
	if (buf[*ct - 1] == BLANK)
		erase1(ttyp, --(*ct), stdout);
	for (--(*ct); *ct >= 0 && buf[*ct] != BLANK; (*ct)--) 
		erase1(ttyp, *ct, stdout);
	(*ct)++;
}

/*------------------------------------------------------------------------
 * strnequ - if s1 and s2 have same length and same value, return TRUE
 *------------------------------------------------------------------------
 */
strnequ(s1, s2, n)
char	*s1, *s2;
int	n;
{
	if (strlen(s1) < n)
		return 0;
	return ! strncmp(s1, s2, n);
}

/*------------------------------------------------------------------------
 * snusage - print usage message
 *------------------------------------------------------------------------
 */
LOCAL snusage(stdout)
int	stdout;
{
	char sb[BUFSIZE];

	sprintf(sb, "usage: snmp agent-mach-name\n");
	write(stdout, sb, strlen(sb));
}

static char *nextchar;		/* points to char that begins next word */

/*------------------------------------------------------------------------
 * initgetword - initialize the routines to get words off the input
 *------------------------------------------------------------------------
 */
LOCAL initgetword(buf)
char	*buf;
{
	nextchar = buf;		/* nextchar at beginnning of buffer */
}

/*------------------------------------------------------------------------
 * getword - get the next word from the input line
 *------------------------------------------------------------------------
 */
LOCAL getword(word)
char	**word;
{
	while (*nextchar == BLANK)	/* skip blanks */
		nextchar++;
	*word = nextchar;
	while (*nextchar != '\0' && *nextchar != BLANK)
		nextchar++;
	if (*nextchar != '\0')
		*nextchar++ = '\0';
}

/*------------------------------------------------------------------------
 * objd2oid - take a object string and parse it into an object id
 *------------------------------------------------------------------------
 */
objd2oid(str, oip)
char		*str;
struct oid	*oip;
{
	register char	*cp;
	struct mib_info	*onp;
	int		idlen, len, i;
	char		tmp[BUFSIZE];

	idlen = 0;
	cp = str;
	if (isalpha(*str)) {
		/*
		 * The string at least starts out with object descriptors,
		 * and may have object ids at the end. So, first, find the
		 * end of the descriptors part.
		 */
		for (; *cp != '\0' && (isalpha(*cp) || *cp == '.'); cp++)
			 /* empty */;
		len = cp - str;
		if (*cp != '\0') 
			len--;

		for (i = 0; i < mib_entries; i++) {
			onp = &mib[i];
			strcpy(tmp, onp->mi_prefix);
			strcat(tmp, onp->mi_name);
			if (strlen(tmp) == len && blkequ(tmp, str, len))
				break;
		}
		if (i >= mib_entries)
			return SYSERR;

		idlen = oip->len = onp->mi_objid.len;
		blkcopy(oip->id, onp->mi_objid.id, idlen*2);

		/* remove possible trailing ".0" */
		if (strequ(cp, "0"))
			return OK;
	}
	/* rest of variable name is object sub-id's */
	if (isdigit(*cp)) 
		oip->len = idlen + dot2oid(&oip->id[idlen], cp);
	return OK;
}

/*------------------------------------------------------------------------
 * dot2oid - convert a character representation of an object id into
 * 	     an array of sub-identifiers
 *------------------------------------------------------------------------
 */
int dot2oid(id, str)
u_short	id[];	/* array of id's */
char	*str;
{
	register int totval;
	int 	numids = 0;
	
	while (TRUE) {
		totval = 0;
		while (isdigit(*str)) 
		    totval = totval * 10 + (*str++ - '0');
		id[numids++] = (u_short) totval;
		
		if (*str == '.')
		    str++;
		else
		    break;
	}
	return numids;
}

/*------------------------------------------------------------------------
 * parseoidlist - read a list of oid's from the input line
 *------------------------------------------------------------------------
 */
parseoidlist(rqdp, word)
struct req_desc	*rqdp;
char		**word;
{
	struct	snbentry *parseoid(), *bl;
	
	while (**word != NULLCH) {
		if ((bl = parseoid(word)) == (struct snbentry *) SYSERR)
			return SYSERR;
		if (rqdp->bindlf == (struct snbentry *) NULL)
			rqdp->bindlf = rqdp->bindle = bl;
		else {
			bl->sb_prev = rqdp->bindle;
			rqdp->bindle = rqdp->bindle->sb_next = bl;
		}
	}
	return OK;
}

/*------------------------------------------------------------------------
 * parseoid - read an oid from the input line
 *------------------------------------------------------------------------
 */
struct snbentry *parseoid(word)
char		**word;
{
	struct	snbentry *bl, *getnewbl();
	
	bl = getnewbl();
	if (objd2oid(*word, &bl->sb_oid) == SYSERR) {
		freemem(bl, sizeof(struct snbentry));
		return (struct snbentry *) SYSERR;
	} else
		SVTYPE(bl) = ASN1_NULL;
	getword(word);
	return bl;
}

static struct oid lastobjid;	/* holds last objid that was returned 	*/
static int lastoidset = FALSE;	/* flags if there is a last objid	*/

/*------------------------------------------------------------------------
 * sendquery - parse the input line and send the query.  Input has
 * one of the following forms:
 * 		 [object-name]+
 * 		 next [object-name]+ 
 * 		 set [object-name type value]+
 *------------------------------------------------------------------------
 */
sendquery(stdout, server)
int	stdout;
char	*server;
{
	struct req_desc	rqd;
	struct snbentry	*bl, *getnewbl();
	char		*word;
	int 		repl;

	initgetword(buf);
	rqd.reqtype = PDU_GET;		/* by default */
	rqd.bindle = rqd.bindlf = (struct snbentry *) NULL;

	getword(&word);
	if (*word == '\0')
		return OK;

	if (strequ(word, "next")) {
		rqd.reqtype = PDU_GETN;
		getword(&word);
		if (parseoidlist(&rqd, &word) == SYSERR) {
			fprintf(stdout, "unknown variable\n\r");
			return SYSERR;
		}
		if ((bl = rqd.bindlf) == (struct snbentry *) NULL) {
			if (lastoidset) { /* no oids so use last one */
				rqd.bindlf = rqd.bindle = bl = getnewbl();
				bl->sb_oid.len = lastobjid.len;
				blkcopy(bl->sb_oid.id, lastobjid.id,
					lastobjid.len*2);
				SVTYPE(bl) = ASN1_NULL;
			} else {
				fprintf(stdout, "bad syntax\n\r");
				return SYSERR;
			}
		}
	} else if (strequ(word, "set")) {
		rqd.reqtype = PDU_SET;
		if (parseset(&rqd, stdout) == SYSERR)
			return SYSERR;
	} else if (parseoidlist(&rqd, &word) == SYSERR) {
		fprintf(stdout, "unknown variable\n\r");
		return SYSERR;
	}
	repl = snclient(&rqd, server, stdout);
	switch (repl) {
	case SCL_OK:
		if (rqd.err_stat == SNMP_OK)
		    snmpprint(stdout, rqd.bindlf);
		else
		    snerr(stdout, &rqd);
		fprintf(stdout, "\r");
		break;
	case SCL_OPENF:
		fprintf(stdout, "snmp: open failed\n\r");
		break;
	case SCL_WRITEF:
		fprintf(stdout, "snmp: write failed\n\r");
		break;
	case SCL_NORESP:
		fprintf(stdout, "snmp: No response from server %s\n\r", server);
		break;
	case SCL_READF:
		fprintf(stdout, "snmp: read failed\n\r");
		break;
	case SCL_BADRESP:
		fprintf(stdout, "snmp: received bad response\n\r");
		break;
	}
	
	/* save this object for use with the "next" operation */
	lastobjid.len = rqd.bindlf->sb_oid.len;
	blkcopy(lastobjid.id, rqd.bindlf->sb_oid.id,
		rqd.bindlf->sb_oid.len*2);
	lastoidset = TRUE;

	snfreebl(rqd.bindlf);
	return OK;
}

/*------------------------------------------------------------------------
 * getnewbl - get a new bindlist node from memory and initialize it
 *------------------------------------------------------------------------
 */
struct snbentry *getnewbl()
{
     	struct snbentry *blp;

	blp = (struct snbentry *) getmem(sizeof(struct snbentry));
	blp->sb_next = blp->sb_prev = 0;
	blp->sb_a1slen = 0;
	return blp;
}

/*------------------------------------------------------------------------
 * parseset - parse the words from the input line for a set operation
 *------------------------------------------------------------------------
 */
parseset(rqdp, stdout)
struct req_desc	*rqdp;
int		stdout;
{
	struct snbentry	*bl, *parseoid();
	char 		*word;

	for (getword(&word); *word != NULLCH; getword(&word)) {
		if ((bl = parseoid(&word)) == (struct snbentry *) SYSERR)
			return SYSERR;
		if (rqdp->bindlf) {
			bl->sb_prev = rqdp->bindle;
			rqdp->bindle = rqdp->bindle->sb_next = bl;
		} else
			rqdp->bindlf = rqdp->bindle = bl;
		if (parsevalue(&word, bl) == SYSERR) {
			fprintf(stdout, "bad syntax\n\r");
			return SYSERR;
		}
	}
	return OK;
}
