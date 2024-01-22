

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
 * PutAfter -- load the buffer previously YANKed after the current line
 */

#include "common.h"

struct line *
PutAfter(CurrLine, cCount)

struct line *CurrLine;
int cCount;

{
        extern struct line *FirstLine, *LastLine, *Buffer,
                           *TopLineOnScreen, *BotLineOnScreen;
        extern Extra *UndoInfo;
        struct line *tptr, *tptr2, *Lptr;
        int i, cLines;

        cLines = cCount * CountLines(Buffer);
        NumLines += cLines;

        if (Buffer == NULL)
           AnnounceError("No lines in buffer.");        
        else {
           CleanUndoInfo(UndoInfo);
           UndoInfo->tag = tagPUT;
           UndoInfo->u.u_delline.prev = CurrLine;
           Lptr = (CurrLine == NULL) ? NULL : CurrLine->next;
           Saved = FALSE;

           for (i = 1; i <= cCount; i++) {
              tptr2 = (CurrLine == NULL) ? NULL : CurrLine->next;
              tptr = CopyLines(Buffer);
              tptr->prev = (tptr2 == NULL) ? LastLine : CurrLine;
              if (tptr2 != NULL) {
                 tptr->prev->next = tptr;
                 tptr2->prev = GetLastLine(tptr);
                 tptr2->prev->next = tptr2;
                 }
              else if (CurrLine == NULL) {
                 TopLineOnScreen = FirstLine = tptr;
                 LastLine = GetLastLine(tptr);
                 }
              else if (tptr->prev == LastLine) {
                 tptr->prev->next = tptr;
                 LastLine = GetLastLine(tptr);
                 BotLineOnScreen = FindBotLine(TopLineOnScreen);
                 }
              else 
                 panic("Unexpected pointer arrangement in PutAfter.");
              }

           UndoInfo->u.u_delline.last = (Lptr == NULL)? LastLine :  
                        Lptr->prev;

           UndoInfo->u.u_delline.first = (CurrLine == NULL) ?
                 FirstLine : CurrLine->next;

           wclear(BotWin);
           wprintw(BotWin, "%d line%s inserted after current line.", cLines,
                (cLines == 1)? "" : "s");
           wrefresh(BotWin);
          
           if (CurrLine == NULL)
              CurrLine = tptr;
           else {
              if (BotLineOnScreen == CurrLine)
                 TopLineOnScreen = TopLineOnScreen->next;
              }

           BotLineOnScreen = FindBotLine(TopLineOnScreen);
           UpdateScreen(TopLineOnScreen, BotLineOnScreen, CurrLine);
        }
   return(CurrLine);
}
