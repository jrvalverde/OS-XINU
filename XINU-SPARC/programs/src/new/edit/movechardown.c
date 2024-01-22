

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
 * MoveCharDown(CurrLine) -- move one character down from the current
 * line.
 */

#include "common.h"

struct line *
MoveCharDown(CurrLine)

struct line *CurrLine;

{
   int i;
   struct line *NewCurrLine;
   extern struct line *BotLineOnScreen;
   extern int BadMove;

   if (CurrLine == NULL || (NewCurrLine = CurrLine -> next) == NULL) {
      AnnounceError("At end of File.\n");
      BadMove = TRUE;
      return(CurrLine);
      }
   else if (CurrLine != BotLineOnScreen) {  /* Don't have to scroll */
      LinePos = MIN(LinePos, NewCurrLine -> length);
      LinePos = MAX(LinePos, 0);
      for (i = 0, CursorX = 0; i <= LinePos; i++)
         CursorX += strlen(unctrl(NewCurrLine->text[i]));
      CursorX = MAX(0, CursorX - 1);
      if (NewCurrLine->text[LinePos] == NULLCHAR)
         CursorX = MAX(0, CursorX - 1);
      wmove(TopWin, ++CursorY, CursorX);
      LineNum++;
      return(NewCurrLine);
      }
   else {
      TopLineOnScreen = TopLineOnScreen -> next;
      CursorY = TOPWINLINES - 1;
      BotLineOnScreen = NewCurrLine;
      LinePos = MIN(LinePos, NewCurrLine -> length);
      LinePos = MAX(LinePos, 0);
      for (i = 0, CursorX = 0; i <= LinePos; i++)
         CursorX += strlen(unctrl(NewCurrLine->text[i]));
      CursorX = MAX(0, CursorX - 1);
      if (NewCurrLine->text[LinePos] == NULLCHAR) 
         CursorX = MAX(0, CursorX - 1);
      wmove(TopWin, CursorY, MAXLINE);
      wprintw(TopWin, "\n");
      wclrtoeol(TopWin);
      scprintw(TopWin, NewCurrLine -> text);
      wmove(TopWin, CursorY, CursorX);
      LineNum++;
      return(NewCurrLine);
      }
}
