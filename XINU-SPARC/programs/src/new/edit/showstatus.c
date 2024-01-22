

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
 * ShowStatus() -- print the status
 */

#include "common.h"

ShowStatus()

{
   extern int NumLines, LineNum, LinePos;

   wclear(BotWin);
   wprintw(BotWin, "Line: %d of %d (%d%%)\n", LineNum, NumLines, 
		(NumLines == 0) ? 0 : (int) 100 * LineNum / NumLines);
   wprintw(BotWin, "Column: %d\n", LinePos + 1);
   wrefresh(BotWin);
}
