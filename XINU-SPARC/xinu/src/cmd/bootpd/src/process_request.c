
/* $Header: /usr/src/local/etc/bootpd/src/RCS/process_request.c,v 1.9 1990/12/05 14:47:25 trinkle Exp $ */

/* 
 * process_request.c - process a bootp request
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 20 12:50:13 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log: process_request.c,v $
 * Revision 1.9  1990/12/05  14:47:25  trinkle
 * Updates from smb, mainly for byte ordering corrects and Ultrix
 * modifications.
 *
 * Revision 1.8  90/07/12  10:50:59  smb
 * Commented and declared register variables.
 * 
 * Revision 1.7  90/07/03  12:44:14  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.6  90/06/27  14:24:01  smb
 * Added call to makename() in place of saving, changing, and restoring
 * the current working directory.
 * 
 * Revision 1.5  90/06/26  16:41:48  smb
 * Added a saving and restoring of the current working directory to handle
 * non-absolute pathnames for config files.
 * 
 * Revision 1.4  90/06/26  15:02:02  smb
 * Added the hardware address to the call to send_reply so that
 * an ARP entry can be manufactured there.
 * Also, the checking of the config file change time will be handled
 * in server().
 * 
 * Revision 1.3  90/06/26  14:47:32  smb
 * Completed.
 * 
 * Revision 1.2  90/06/21  16:57:20  smb
 * Just about finished, still need to check for changed config file.
 * 
 * Revision 1.1  90/06/20  16:33:15  smb
 * Initial revision
 * 
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <syslog.h>
#include <strings.h>
#include <hash.h>
#include <bootp.h>
#include <bootp_conf.h>
#include <null.h>

#define WORLD_READ	0004

/* these are for Ultrix */
#ifndef LOG_INFO
#define LOG_INFO 6
#endif
#ifndef LOG_WARNING
#define LOG_WARNING 4
#endif

extern unsigned long my_best_ipaddr();
extern char *makename();

int process_request(msg)
     register struct bootp_msg *msg;
{
    register struct bootp_entry *entry;
    struct generic_pair *generic;
    register struct vendor *vend;
    char *bootfile, en[SZENTEXT], *filename, *hdir, *rootdir = "/";
    struct stat stat_buf;
    int filesize;
    unsigned long saddr;

    /* the check for a changed config file is in server() 	  */
    
    /* start with the hardware type - see if it one we can handle */
    switch (msg->htype) {
	case ETHERNET: {
	    entry = (struct bootp_entry *) lookup(haddr_hash, msg->chaddr,
						  msg->hlen);
	    if (!entry) {
		return(-1);
	    }
	    sprint_en(en, msg->chaddr);
	    syslog(LOG_INFO, "found entry for %s", en);
	    break;
	}
	default: {
	    syslog(LOG_WARNING, "unknown hardware type %d", msg->htype);
	    return(-1);
	    break;
	}
    }

    /* at this point, we have an entry. begin filling in the reply fields */

    /*
     * if the client has included its IP address, then we believe it
     * and don't bother setting the one from the database - otherwise,
     * use the one we know 
     */
    if (ntohl(msg->ciaddr) == 0) {
	msg->yiaddr = htonl(entry->ipaddr);
    }
    else {
	msg->yiaddr = msg->ciaddr;
    }

    /*
     * find a boot file to use for the client.  basically, this is one
     * of the following:
     * 	1) the client gave us a generic name, look it up in the
     * 	   generic hash
     * 	2) the client gave us a real file name, use it
     * 	3) the client's entry specifies a boot file name, use it
     * 	4) the client's entry specifies a generic name, look it up and
     * 	   use it
     */
    if (msg->file[0] != '\0') {
	generic = (struct generic_pair *) lookup(generic_hash, msg->file,
						 strlen(msg->file));
	if (generic) {
	    bootfile = (char *) xmalloc(strlen(generic->file_name) + 1);
	    strcpy(bootfile, generic->file_name);
	}
	else {
	    bootfile = (char *) xmalloc(strlen(msg->file) + 1);
	    strcpy(bootfile, msg->file);
	}
    }
    else {
	if (entry->bootfile) {
	    bootfile = (char *) xmalloc(strlen(entry->bootfile) + 1);
	    strcpy(bootfile, entry->bootfile);
	}
	else if (entry->generic) {
	    bootfile = (char *)	xmalloc(strlen(entry->generic->file_name) + 1);
	    strcpy(bootfile, entry->generic->file_name);
	}
	else {
	    syslog(LOG_ERR, "no generic/boot file specified or requested");
	    return(-1);
	}
    }

    /*
     * now that we have a file name, lets look for it. if the first stat()
     * fails, we also need to stat() the file name with ".<machine>"
     * appended, if the entry specifies a machine type.
     */
    if (entry->homedir) {
	hdir = entry->homedir;
    }
    else {
	hdir = rootdir;
    }
    filename = makename(hdir, bootfile, NULL);
    
    if (stat(filename, &stat_buf) < 0) {
	filename = makename(hdir, bootfile, entry->machine);
	if (stat(filename, &stat_buf) < 0) {
	    syslog(LOG_NOTICE, "stat %s: %m", filename);
	    return(-1);
	}
    }

    /*
     * the file exists, check to make sure it is world readable - this
     * ensures that the tftp daemon process will be able to get it and
     * return it to the client.  also, we should check that it is not
     * some kind of special file.
     */
    if (!(stat_buf.st_mode & WORLD_READ)) {
	syslog(LOG_NOTICE, "file %s is not world readable", filename);
	return(-1);
    }
    if ((stat_buf.st_mode & S_IFMT) != S_IFREG) {
	syslog(LOG_NOTICE, "file %s is not a regular file", filename);
	return(-1);
    }

    /* the file was ok, copy the name to the message and save its length */
    strcpy(msg->file, filename);
    filesize = stat_buf.st_size;
    free(bootfile);

    /*
     * at this point, we still need to process the siaddr field, and
     * the vendor field and send it back out.  don't forget to change
     * the op to a reply.
     */
    vend = (struct vendor *) msg->vend;
    switch (ntohl(vend->magic)) {
	case 0:			/* no cookie specified */
	case RFC1084: {
	    process_1084_vendor(vend, entry, filesize);
	    break;
	}
	case XINU: {
	    process_xinu_vendor(vend, entry, filesize);
	    break;
	}
	default: {
	    syslog(LOG_INFO, "unknown magic cookie 0x%x", ntohl(vend->magic));
	    break;
	}
    }

    if ((saddr = my_best_ipaddr(msg->yiaddr)) == 0) {
	return(-1);
    }
    msg->siaddr = saddr;
    msg->op = BOOTREPLY;
    if (msg->ciaddr != 0) {
	return(send_reply((char *) msg, msg->ciaddr, msg->chaddr));
    }
    else {
	return(send_reply((char *) msg, msg->yiaddr, msg->chaddr));
    }
}

