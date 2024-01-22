

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
 * GotoEndOfFile() -- make the last line in the file the current line.
 */

#include "common.h"

struct line *
GotoEndOfFile()

{
   struct line *Lptr;
   int cLines;
   extern struct line *CurrLine, *LastLine;

   if (LastLine == NULL) 
      return(LastLine);
   CurrLine = LastLine;
   LinePos = CursorX = 0;
   LineNum = NumLines;

   if (BotLineOnScreen != LastLine) {
      CursorY = TOPWINLINES - 1;
      BotLineOnScreen = LastLine;
      TopLineOnScreen = FindTopLine(BotLineOnScreen);

      UpdateScreen(TopLineOnScreen, BotLineOnScreen, LastLine);
      }
   else {
      for (Lptr = TopLineOnScreen, cLines = 1; cLines < TOPWINLINES &&
         Lptr != NULL; Lptr = Lptr -> next, cLines++);
		/* and I really mean NULL */

      CursorY = (Lptr == NULL) ? cLines - 2 : TOPWINLINES -1;
      wmove(TopWin, CursorY, 0);
   }

   return(LastLine);

}
