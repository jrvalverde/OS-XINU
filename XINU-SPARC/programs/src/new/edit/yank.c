

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
 * Yank -- grab the next n lines
 */

#include "common.h"

Yank(cLines)
int cLines;

{
   extern struct line *Buffer;
   struct line *FreeLines(), *CopyLine();
   struct line *tptr, *tptr2;
   int i;

   if (CurrLine == NULL) 
      AnnounceError("No lines to yank.");
   else {
   wclear(BotWin);
   if (cLines > 0) {
      Buffer = FreeLines(Buffer);
      i = 1;
      tptr = Buffer = CopyLine(CurrLine);
      Buffer->prev = Buffer-> next = NULL;
      for (i = 2, tptr2 = CurrLine->next; i <= cLines && tptr2 != NULL;
                   i++, tptr2 = tptr2->next) {
		/* I mean NULL here too... */
         tptr->next = CopyLine(tptr2);
         tptr->next->prev = tptr;
         tptr = tptr-> next;
         tptr->next = NULL;
         }

      if (i > 2) 
         wprintw(BotWin, "%d lines yanked into buffer.", i-1);
      else
         wprintw(BotWin, "1 line yanked into buffer.");
      }
   else {
      wprintw(BotWin, "No lines yanked.");
      }
   }
   wrefresh(BotWin);
   
}
