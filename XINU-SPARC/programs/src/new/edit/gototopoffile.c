

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
 * GotoTopOfFile() -- make the current line the first line.
 */

#include "common.h"

struct line *
GotoTopOfFile()

{
   extern struct line *CurrLine, *FirstLine;

   LinePos = CursorX = CursorY = 0;
   CurrLine = FirstLine;
   LineNum = 1;

   if (TopLineOnScreen != FirstLine) {
      TopLineOnScreen = FirstLine;
      BotLineOnScreen = FindBotLine(TopLineOnScreen);
      UpdateScreen(TopLineOnScreen, BotLineOnScreen, CurrLine);
   }
   else {
      wmove(TopWin, 0, 0);
   }

   return(CurrLine);
      
}
