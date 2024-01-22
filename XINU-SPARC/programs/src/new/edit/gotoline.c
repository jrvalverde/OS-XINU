

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
 * GotoLine(CurrtLine) -- goto the nth line of the file.
 */

#include "common.h"

struct line *
GotoLine(CurrLine, cLine)

struct line *CurrLine;
int cLine;

{
   extern int CursorX, CursorY, LineNum, NumLines;
   int forward;

   wclear(BotWin);
   wprintw(BotWin, "Enter line number to go to: ");
   wrefresh(BotWin);
   cLine = wgetnum(BotWin); 

   if (cLine > NumLines) {
      AnnounceError("That would require going past end of file.");
      return(CurrLine);
   }
   else if (cLine == 0)
      return(CurrLine);
   else if (cLine < 0) {
      AnnounceError("No such line.");
      return(CurrLine);
   }

   CursorX = 0;

   forward = cLine > LineNum;

   if (forward)
      while (cLine != LineNum && CurrLine != LastLine) {
         LineNum++;
         CurrLine = CurrLine->next;
         }
   else
      while (cLine != LineNum && CurrLine != FirstLine) {
         LineNum--;
         CurrLine = CurrLine->prev;
         }

   if (! IsOnScreen(CurrLine)) {
      if (forward) {
         BotLineOnScreen = CurrLine;
         TopLineOnScreen = FindTopLine(CurrLine);
         }
      else {
         TopLineOnScreen = CurrLine;
         BotLineOnScreen = FindBotLine(CurrLine);
         }
      }
         
   UpdateScreen(TopLineOnScreen, BotLineOnScreen, CurrLine);

   return(CurrLine);

}
