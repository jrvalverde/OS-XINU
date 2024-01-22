
/* 
 * wyse50.c - set terminal type to wyse50
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Sun Jun 24 22:50:04 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

#include <stdio.h>
#include <sys/file.h>

extern char **environ;

#define termstring "TERM="
#define termcapstring "TERMCAP="

#define TERMWYSE50	"TERM=wyse50"
#define TERMCAPWYSE50	"TERMCAP=w2|wyse50|wyse50na:vb=\\EA04\\200\\200\\200\\200\\200\\200\\200\\200\\200\\200\\EA00:is=\\EA00\\EA10\\EA21\\EA31\\E\\1401\\E\\1407\\E\\1409\\E\\140\\:\\E\\140@\\E0:ic=\\EQ:cr=^M:do=^J:nl=^J:bl=^G:al=\\EE:am:le=^H:bs:ce=\\ET:cm=\\E=%+\\040%+\\040:cl=\\E*:cd=\\EY:co#80:dc=\\EW:dl=\\ER:ho=^^:kn#16:k0=^A@\\r:k1=^AA\\r:k2=^AB\\r:k3=^AC\\r:k4=^AD\\r:k5=^AE\\r:k6=^AF\\r:k7=^AG\\r:k8=^AH\\r:k9=^AI\\r:l0=F1:l1=F2:l2=F3:l3=F4:l4=F5:l5=F6:l6=F7:l7=F8:l8=F9:l9=F10:kh=^^:li#24:ma=^Hh^Jj^Kk^Ll:mi:nd=^L:se=\\EG0:so=\\EG4:sg#1:ue=\\EG0:us=\\EG8:ug#1:up=^K:bt=\\EI:bw:da:db:eo:i2=\\EA1p:hs:fs=\\r:ts=\\EA1p\\Ef:ds=\\Ez(\\r:sl#78:"

termwyse50()
{
	int i;

	printf("setting terminal type to wyse50\n");
	i = 0;
        while (environ[i]!=NULL) {
		if (strncmp(environ[i], termstring, strlen(termstring))
		    == 0) {
			strcpy(environ[i], TERMWYSE50);
		}
		else if (strncmp(environ[i], termcapstring,
				 strlen(termcapstring)) == 0) {
			strcpy(environ[i], TERMCAPWYSE50);
		}
		i++;
	}
}
