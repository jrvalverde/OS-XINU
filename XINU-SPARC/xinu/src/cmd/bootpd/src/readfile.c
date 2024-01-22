
/* $Header: /usr/src/local/etc/bootpd/src/RCS/readfile.c,v 2.6 1990/12/05 14:47:45 trinkle Exp $ */

/* 
 * readfile.c - read and parse the configuration file
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 12 12:29:38 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: readfile.c,v $
 * Revision 2.6  1990/12/05  14:47:45  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 2.5  90/07/12  10:51:09  smb
 * Commented and declared register variables.
 * 
 * Revision 2.4  90/07/03  12:45:07  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 2.3  90/06/26  16:39:53  smb
 * Added a reference/link count to entries in the name hash
 * 
 * Revision 2.2  90/06/19  14:42:25  smb
 * Added declaration for mtime, the last file modification time for 
 * the configuration file.
 * 
 * Revision 2.1  90/06/14  16:50:52  smb
 * Worked out the bugs and moved a lot of code to separate files.  It has
 * now been tested, some, and seems to work okay.
 * 
 * Revision 2.0  90/06/13  14:28:38  smb
 * New version.  This one has the FSM implemented and uses the new data
 * structures that share entries.  It compiles but is untested.
 * 
 * Revision 1.2  90/06/13  11:19:40  smb
 * Reworked to build data structure.
 * Not tested and incomplete.
 * 
 * Revision 1.1  90/06/12  14:03:15  smb
 * Initial revision
 * 
 */

#include <stdio.h>
#include <syslog.h>
#include <strings.h>
#include <bool.h>

#include <bootp_conf.h>
#include <hash.h>

long mtime = 0;

int readfile(name)
     char *name;
{
    char linebuf[SZMAXLINE], taddr[2*SZMAXHADDR];
    register char token[SZMAXTOKEN];
    FILE *configf;
    int i, j, line, len, stat, tag;
    int state;
    char *tvalue, *tname;
    struct generic_pair *tgeneric;
    register struct bootp_entry *tentry;
    register struct list *lptr;
    Bool alias;
    

    if (!(configf = fopen(name, "r"))) {
	syslog(LOG_ERR, "unable to open %s", name);
	return(-1);
    }

    state = INIT;
    j = 0;
    line = 0;

    while (fgets(linebuf, SZMAXLINE, configf)) {
	line++;
	i = 0;
	len = strlen(linebuf);
	if (len <= 1) {		/* skip blank lines 	*/
	    continue;
	}

	while (i<len) {
	    if (j >= SZMAXTOKEN) {
		syslog(LOG_ERR, "token too long on line %d", line);
		break;
	    }
	    if (linebuf[i] == '#') {
		i = len - 1; 	/* skip to EOLN */
	    }
	    switch (state) {
		case INIT: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n' : {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    break;
				}
			    }
			    break;
			}
			case '\n' :
			case ' '  :
			case '\t' : {
			    i++;
			    break;
			}
			case '%' : {
			    i++;
			    state = IDLE;
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    state = GEN_READ;
			    break;
			}
		    }
		    break;
		}
		case GEN_READ: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n' : {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    break;
				}
			    }
			    break;
			}
			case '\n' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token '\n' in state GEN_READ",
				   line);
			    break;
			}
			case ' '  :
			case '\t' : {
			    token[j] = '\0';
			    tgeneric = (struct generic_pair *)
				xmalloc(sizeof(struct generic_pair));
			    bzero(tgeneric, sizeof(struct generic_pair));
			    tgeneric->generic_name =
				(char *) xmalloc(strlen(token)+1);
			    strcpy(tgeneric->generic_name, token);
			    j = 0;
			    state = SKIP;
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    break;
			}
		    }
		    break;
		}
		case SKIP: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n' : {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    state = FILE_READ;
				    break;
				}
			    }
			    break;
			}
			case '\n' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token '\n' in state SKIP",
				   line);
			    break;
			}
			case ' '  :
			case '\t' : {
			    i++;
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    state = FILE_READ;
			    break;
			}
		    }
		    break;
		}
		case FILE_READ: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    break;
				}
			    }
			    break;
			}
			case '\n' :
			case ' '  :
			case '\t' : {
			    token[j] = '\0';
			    tgeneric->file_name =
				(char *) xmalloc(strlen(token)+1);
			    strcpy(tgeneric->file_name, token);
			    j = 0;
			    stat = insert(tgeneric, generic_hash,
					  tgeneric->generic_name,
					  strlen(tgeneric->generic_name));
			    if (stat < 0) {
				syslog(LOG_WARNING, "nonunique generic name %s",
				       tgeneric->generic_name);
			    }
			    state = (linebuf[i] == '\n') ? INIT : SKIP_EOL;
			    i++;
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    break;
			}
		    }
		    break;
		}
		case SKIP_EOL: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    state = ERROR;
				    syslog(LOG_ERR,
					   "conf %d: bad token after '//' in state SKIP_EOL",
					   line);
				    break;
				}
			    }
			    break;
			}
			case '\n' : {
			    i++;
			    state = INIT;
			    break;
			}
			case ' '  :
			case '\t' : {
			    i++;
			    break;
			}
			default: {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token in state SKIP_EOL",
				   line);
			    break;
			}
		    }
		    break;
		}
		case IDLE: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    state = READ_NAME;
				    alias = FALSE;
				    break;
				}
			    }
			    break;
			}
			case ' '  :
			case '\t' :
			case '\n' : {
			    i++;
			    break;
			}
			case '=' :
			case '|' :
			case ':' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token ':' in state IDLE",
				   line);
			    break;
			}
			default: {
			    token[j++] = linebuf[i++];
			    state = READ_NAME;
			    alias = FALSE;
			    break;
			}
		    }
		    break;
		}
		case READ_NAME: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    break;
				}
			    }
			    break;
			}
			case ' '  :
			case '\t' : {
			    i++;
			    break;
			}
			case ':' :
			case '|' : {
			    token[j] = '\0';
			    tname = (char *) xmalloc(strlen(token) + 1);
			    strcpy(tname, token);
			    if (alias) {
				atend(tname, &tentry->aliases);
			    }
			    else {
				tentry = (struct bootp_entry *)
				    xmalloc(sizeof(struct bootp_entry));
				bzero(tentry, sizeof(struct bootp_entry));
				tentry->name = tname;
				alias = TRUE;
			    }
			    state = (linebuf[i] == ':') ? READY : READ_NAME;
			    j = 0;
			    i++;
			    break;
			}
			case '='  :
			case '\n' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token '\n' in state READ_NAME",
				   line);
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    break;
			}
		    }
		    break;
		}
		case READY: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    state = READ_TAG;
				    break;
				}
			    }
			    break;
			}
			case ' '  :
			case '\t' :
			case ':'  : {
			    i++;
			    break;
			}
			case '|' :
			case '=' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token '=' in state READY",
				   line);
			    break;
			}
			case '\n' : {
			    i++;
			    state = IDLE;
			    stat = insert(tentry, name_hash, tentry->name,
				   strlen(tentry->name));
			    if (stat < 0) {
				syslog(LOG_WARNING, "nonunique entry name %s",
				       tentry->name);
			    }
			    else {
				tentry->rcount = 1;
			    }
			    lptr = tentry->aliases;
			    while (lptr) {
				stat = insert(tentry, name_hash,
					      lptr->data,
					      strlen(lptr->data));
				if (stat < 0) {
				    syslog(LOG_WARNING,
					   "nonunique alias name %s",
					   lptr->data);
				}
				else {
				    tentry->rcount++;
				}
				lptr = lptr->next;
			    }
			    if (tentry->hlen != 0) {
				stat = insert(tentry, haddr_hash,
					      tentry->haddr, tentry->hlen);
				if (stat < 0) {
				    for (j=0; j<tentry->hlen*2; j+=2) {
					sprintf(&taddr[j], "%02x",
						tentry->haddr[j/2]);
				    }
				    syslog(LOG_WARNING,
					   "nonunique hardware address %s",
					   taddr);
				}
			    }
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    state = READ_TAG;
			    break;
			}
		    }
		    break;
		}
		case READ_TAG: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    break;
				}
			    }
			    break;
			}
			case ' '  :
			case '\t' : {
			    i++;
			    break;
			}
			case '|'  :
			case ':'  :
			case '\n' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token '\n' in state READ_TAG",
				   line);
			    break;
			}
			case '=' : {
			    token[j] = '\0';
			    j = 0;
			    tag = findtag(token);
			    if (tag < 0) {
				state = ERROR;
				syslog(LOG_ERR, "bad tag '%s' on line %d",
				       token,line);
			    }
			    else {
				state = READ_VALUE;
				i++;
			    }
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    break;
			}
		    }
		    break;
		}
		case READ_VALUE: {
		    switch (linebuf[i]) {
			case '\\' : {
			    i++;
			    switch (linebuf[i]) {
				case '\n': {
				    i++;
				    break;
				}
				default: {
				    token[j++] = linebuf[i++];
				    break;
				}
			    }
			    break;
			}
			case ' '  :
			case '\t' : {
			    i++;
			    break;
			}
			case '='  :
			case '\n' : {
			    state = ERROR;
			    syslog(LOG_ERR,
				   "conf %d: bad token '\n' in state READ_VALUE",
				   line);
			    break;
			}
			case ':' :
			case '|' : {
			    token[j] = '\0';
			    tvalue = (char *) xmalloc(strlen(token) + 1);
			    strcpy(tvalue, token);
			    j = 0;
			    state = enter(tag, &tvalue, tentry, linebuf[i]);
			    if (state == ERROR) 
				syslog(LOG_ERR,
				       "conf %d: bad state from enter(tag=%d)",
				       line, tag);
			    i++;
			    break;
			}
			default : {
			    token[j++] = linebuf[i++];
			    break;
			}
		    }
		    break;
		}
		case ERROR: {
		    syslog(LOG_ERR, "misc conf file error, about line %d: '%s'",
			   line, linebuf);
		    return(-1);
		    break;
		}
		default: {
		    syslog(LOG_ERR, "logic error");
		    return(-1);
		    break;
		}
	    }
	}
    }
    if (state != IDLE) { /* we have premature EOF */
        syslog(LOG_ERR, "unexpected EOF at line %d\n", line);
    }
    fclose(configf);

    mtime = getmtime(name);
    return(0);
}

	
