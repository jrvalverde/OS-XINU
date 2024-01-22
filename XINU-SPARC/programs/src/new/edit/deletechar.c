
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
 * DeleteChar(CurrentLine) -- delete the character under the cursor from
 * the current line.
 */

#include "common.h"

struct line *
DeleteChar(CurrentLine, cCount, UserCall)

struct line *CurrentLine;
int cCount;
int UserCall;

{
   extern struct line *MeldLines();
   char c;
   char *Calloc();
   extern Extra *UndoInfo;
   struct line *Lptr;
   extern int BadMove;
   int ii, j = 0;

   if (CurrentLine == NULL || cCount == 0) {
      AnnounceError("No character to delete.\n");
      return(CurrentLine);
   }

   if (UserCall) {
      CleanUndoInfo(UndoInfo);
      UndoInfo->tag = tagDELETECHAR;
      UndoInfo->u.u_delchar.first = CurrentLine;
      UndoInfo->u.u_delchar.bLp = LinePos;
      UndoInfo->u.u_delchar.del = (struct line *) Calloc(sizeof(struct line));
      Lptr = UndoInfo->u.u_delchar.del;
   }

   for (ii = 1; ii <= cCount; ii++) {
      if (CurrentLine == NULL) {
         AnnounceError("No character to delete.\n");
         break;
         } 
      else if (CurrentLine->text[LinePos] == NULLCHAR) {
         int iii;

         for (iii = LinePos; iii < MAXLINE; iii++)
            CurrentLine->text[iii] = NULLCHAR;

         CurrentLine = MeldLines(CurrentLine);
         if (BadMove) break;
         if (UserCall) {
            Lptr->next = (struct line *) Calloc(sizeof(struct line));
            Lptr->next->prev = Lptr;
            Lptr->length = j;
            j = 0;
            Lptr = Lptr->next;
            }
         }
      else {
         int i;

         if (strlen(unctrl(CurrentLine->text[LinePos])) == 2) {
            wmove(TopWin, CursorY, --CursorX);
            wdelch(TopWin);
            }
      
         wdelch(TopWin);

         /* copy the deleted char into the save buffer */
         if (UserCall)
            Lptr->text[j++] = CurrentLine->text[LinePos]; 

         for (i = LinePos; i <= CurrentLine -> length; i++)
            CurrentLine->text[i] = CurrentLine->text[i+1];

         c = CurrentLine->text[LinePos];
         if((strlen(unctrl(c)) == 2) && (c != NULLCHAR))
            CursorX++;

         CurrentLine->length -= 1;

         wmove(TopWin, CursorY, CursorX);
         }
   }

      if (UserCall) {
         Lptr->length = strlen(Lptr->text);
         UndoInfo->u.u_delchar.last = UndoInfo->u.u_delchar.first;
         }

      BotLineOnScreen = FindBotLine(TopLineOnScreen);
      UpdateScreen(TopLineOnScreen, BotLineOnScreen, CurrLine);

      return(CurrentLine);

}
         
