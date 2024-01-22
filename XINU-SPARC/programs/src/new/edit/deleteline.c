
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
 * DeleteLine(Lptr, cCount, UserCall) -- removes cCount lines from the screen 
 * and the file.  UserCall is a boolean that is true if the proc was called
 * from GetCommand and not internally from the editor.
 */

#include "common.h"

struct line *
DeleteLine(Lptr, cCount, UserCall)

struct line *Lptr;
int cCount;
int UserCall;

{
   struct line *tptr = NULL;
   struct line *LastDel;
   extern Extra *UndoInfo;
   extern struct line *FirstLine, *LastLine, *TopLineOnScreen, *BotLineOnScreen;
   extern int CursorY, CursorX, LinePos, NumLines, LineNum;
   int i;

   i = CountLines(Lptr);
   cCount = MIN(cCount, i);

   if (Lptr == NULL || cCount == 0) {
      AnnounceError("No line to delete.");
      return(Lptr);
   }

   if (UserCall) {
      CleanUndoInfo(UndoInfo);
      UndoInfo->tag = tagDELETELINE;
      UndoInfo->u.u_delline.prev = Lptr->prev;
      UndoInfo->u.u_delline.first = Lptr;
      }

   LinePos = CursorX = 0;

   for (LastDel = Lptr, i = 1; i < cCount && LastDel->next != NULL; 
        i++, LastDel = LastDel->next) 

   UndoInfo->u.u_delline.last = LastDel;

   if (Lptr == FirstLine) 
      if (LastDel == LastLine) { /* deleting the whole file */
         tptr = FirstLine = LastLine = 
                TopLineOnScreen = BotLineOnScreen = NULL;
         LineNum = 1;
         CursorY = 0;
      }
      else {
         tptr = TopLineOnScreen = FirstLine = LastDel->next;
         tptr -> prev = NULL;
      }
   else 
      if (LastDel == LastLine) {
         tptr = BotLineOnScreen = LastLine = Lptr -> prev;
         if (TopLineOnScreen == Lptr) 
	    TopLineOnScreen = FindTopLine(BotLineOnScreen);
         tptr -> next = NULL;
      }
      else {       /* a 'normal' line */
         if (Lptr == TopLineOnScreen) 
            TopLineOnScreen = LastDel -> next;
         LastDel->next->prev = Lptr->prev;
         Lptr->prev->next = LastDel->next;
         tptr = LastDel->next;
      }
   
   BotLineOnScreen = FindBotLine(TopLineOnScreen);
   NumLines -= cCount;
   LineNum = MIN(LineNum, NumLines);

   if (UserCall) {
      LastDel->next = NULL;
      if (UndoInfo == NULL)
         panic("Unexpected null pointer in DeleteLine.");
      else
         UndoInfo->u.u_delline.last = LastDel;
      }

   UpdateScreen(TopLineOnScreen, BotLineOnScreen, tptr);
   wclear(BotWin);
   wprintw(BotWin, "%d line%s of text deleted.", cCount, 
      (cCount == 1) ? "" : "s");
   wrefresh(BotWin);
   return(tptr);

}
