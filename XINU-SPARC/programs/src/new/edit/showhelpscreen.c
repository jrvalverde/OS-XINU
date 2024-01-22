   

/*
 *   PCE: Programming Contest Editor
 *
 *   Copyright (c) 1986, Steve J. Chapin 
 *
 *	Steve Chapin
 *	Department of Computer Sciences
 * 	Purdue University
 *	West Lafayette, IN  47907
 *   	sjc@mordred.cs.purdue.edu
 *
 *   Permission is hereby granted to use this program for the purposes
 *   of holding programming contests, especially those sponsored by
 *   the Association for Computing Machinery, and to make changes to
 *   the program, as long as this copyright notice is included in its entirety 
 *   and credit is given to the original author.  Basically, I put in
 *   a lot of time on this mother, and I don't want anyone who hacks
 *   it up to claim it as theirs.
 *
 *   No warranties, expressed or implied, are in effect for this software.
 *   Use it at your own risk.
 */

/*
 * ShowHelpScreen() -- show the help text
 */

#include "common.h"

ShowHelpScreen()

{
   extern int ClearBotWin;

   ClearBotWin = TRUE;
   touchwin(HelpWin);
   wrefresh(HelpWin);

   wclear(BotWin);
   wprintw(BotWin, "Hit any key to return to editing...");
   wrefresh(BotWin);
   wgetch(BotWin);
   
   wclear(BotWin);
   wrefresh(BotWin);
   touchwin(TopWin);
}
