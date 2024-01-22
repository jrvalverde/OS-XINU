   

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
 * MeldLines(CurrLine) -- Join the current line with the next
 */

#include "common.h"

struct line *
MeldLines(CurrLine)

struct line *CurrLine;

{
   struct line *tptr;
   extern int LinePos;
   extern int BadMove;
   extern struct line *LastLine;

   if (CurrLine == LastLine || CurrLine == NULL || CurrLine->next == NULL) {
      AnnounceError("At end of file.  No following line to join.");
      BadMove = TRUE;
      }
   else
   if (CurrLine->length + CurrLine->next->length >= MAXLINE) {
      AnnounceError("Joining would result in an overly long line.");
      BadMove = TRUE;
      }
   else {
      int saveX = CurrLine -> length-1; 
      int j;
      tptr = CurrLine -> next;
      
      CurrLine -> length += tptr -> length;

      for (j = 0; j <= tptr->length; j++)
         CurrLine->text[saveX + 1 + j] = tptr->text[j];

      wmove(TopWin, CursorY, 0);
      scprintw(TopWin, CurrLine -> text);
      
      wmove(TopWin, ++CursorY, 0);
      CurrLine->next = DeleteLine(tptr, 1, FALSE);
      if (CurrLine == LastLine) {
         CursorY++;
         CurrLine->next = NULL;
      }
   
      if (CurrLine -> next == NULL) LastLine = CurrLine;

      for (j = CursorX = 0; j <= saveX; j++)
         CursorX += strlen(unctrl(CurrLine->text[j]));

      LinePos = saveX+1;
      wmove(TopWin, --CursorY, CursorX);
   }

   return(CurrLine);

}
