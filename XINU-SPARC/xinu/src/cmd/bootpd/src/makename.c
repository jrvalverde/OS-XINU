
/* $Header: /.gwen/u5/smb/src/bootp/bootps/src/RCS/makename.c,v 1.3 90/07/12 10:50:37 smb Exp $ */

/* 
 * makename.c - takes a path, a filename, and an extension and cats them
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jun 27 09:45:07 1990
 *
 * Copyright (c) 1990 Scott M Ballew and Cypress Network Operations Center
 *
 */

/*
 * $Log:	makename.c,v $
 * Revision 1.3  90/07/12  10:50:37  smb
 * Commented and declared register variables.
 * 
 * Revision 1.2  90/07/03  12:43:58  smb
 * Restructured directories to move header files to ../include
 * 
 * Revision 1.1  90/06/27  14:23:38  smb
 * Initial revision
 * 
 */

#include <strings.h>
#include <bootp.h>
#include <null.h>

static char filename[SZFNAME];

char *makename(path, file, ext)
     register char *path, *file, *ext;
{
    int len = 0;

    if (path) {
	len += strlen(path);
    }
    if (file) {
	len += strlen(file);
    }
    if (ext) {
	len += strlen(ext);
    }
    if (len > SZFNAME - 3) {
	return(NULL);
    }
    filename[0] = '\0';
    if (path) {
	strcpy(filename, path);
    }
    if (file) {
	if (filename[0] != '\0') {
	    strcat(filename, "/");
	    strcat(filename, file);
	}
	else {
	    strcpy(filename, file);
	}
    }
    if (ext) {
	if (filename[0] != '\0') {
	    strcat(filename, ".");
	    strcat(filename, ext);
	}
	else {
	    strcpy(filename, ext);
	}
    }
    return(filename);
}

