
/* $Header: /usr/src/local/etc/bootpd/include/RCS/bootp_conf.h,v 2.6 90/07/27 10:34:59 trinkle Exp $ */

/* 
 * bootp_conf.h - structures for configuration storage
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue Jun 12 13:01:18 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	bootp_conf.h,v $
 * Revision 2.6  90/07/27  10:34:59  trinkle
 * Xinu additions from smb.
 * 
 * Revision 2.5  90/07/12  09:58:39  smb
 * Commented.
 * 
 * Revision 2.4  90/07/03  12:50:46  smb
 * Restructured directories to move all header files here.
 * 
 * Revision 2.3  90/06/26  16:42:36  smb
 * Added a reference/link count to the bootp_entry structure for use
 * by reload().
 * 
 * Revision 2.2  90/06/19  14:41:53  smb
 * Added declaration for mtime, the last file modification time for 
 * the configuration file.
 * 
 * Revision 2.1  90/06/14  16:44:33  smb
 * Moved many of the pieces of this file to separate header files.
 * 
 * Revision 2.0  90/06/13  14:27:50  smb
 * New version.  This one has the FSM implemented and uses the new data
 * structures that share entries.  It compiles but is untested.
 * 
 * Revision 1.2  90/06/13  13:15:49  smb
 * Added a few fields, about to change data structures (again).
 * 
 * Revision 1.1  90/06/12  14:44:19  smb
 * Initial revision
 * 
 */

#ifndef SIZES_H
#include <sizes.h>
#endif

#ifndef LIST_H
#include <list.h>
#endif

/* definitions of states for the FSM */
#define INIT		0		/* initial state		*/
#define GEN_READ	1		/* reading a generic name	*/
#define SKIP		2		/* skipping blanks or tabs	*/
#define FILE_READ	3		/* reading a file name		*/
#define SKIP_EOL	4		/* looking for EOLN		*/
#define IDLE		5		/* waiting for machine name	*/
#define READ_NAME	6		/* reading machine name		*/
#define READY		7		/* waiting to read tag fields	*/
#define READ_TAG	8		/* reading a field tag		*/
#define READ_VALUE	9		/* reading a field value	*/
#define ERROR		10		/* bad news			*/

/* structure to store a <generic_name, file_name> pair */
struct generic_pair {
    char *generic_name;
    char *file_name;
};

/* structure of an entry in the haddr and name hash tables */
struct bootp_entry {
    int rcount;				/* count of references to entry */
    char haddr[SZMAXHADDR];		/* only hosts will have these 3	*/
    short hlen;
    short htype;
    char *name;				/* everyone will have a name	*/
    struct list *aliases;		/* some will have more		*/
    unsigned long ipaddr;		/* only hosts will have one	*/
    unsigned long netmask;
    struct list *gateway;
    struct list *lpr_servers;
    struct list *rlp_servers;
    struct list *domain_servers;
    struct list *time_servers;
    long time_offset;
    char *homedir;
    char *bootfile;
    struct generic_pair *generic;
    char *machine;
/* begin Xinu additions */
    unsigned long cnetmask;		/* Cypress network mask		*/
    unsigned long cipaddr;		/* Cypress IP address		*/
    unsigned long cimplets;		/* Cypress implet list (4)	*/
};

/* extern decl for the last time the config file was modified */
extern long mtime;
