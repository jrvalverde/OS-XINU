      

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
 * MoveCharLeft(CurrLine) -- move one character left on the current
 * line.
 */

#include "common.h"

struct line *
MoveCharLeft(CurrLine) 

struct line *CurrLine;

{
   struct line *MoveCharUp();
   extern int BadMove;

   /* first, are we at beginning of line? */
   if (LinePos == 0) {
      if (CurrLine == FirstLine) {
         AnnounceError("At beginning of file.");
         BadMove = TRUE;
         }
      else {
         CurrLine = MoveCharUp(CurrLine);
         GotoEndOfLine(CurrLine);
         }
      }
   else {
      CursorX -= strlen(unctrl(CurrLine->text[LinePos]));
      if (CurrLine->text[LinePos] == NULLCHAR)
         CursorX++;
      LinePos--;
      wmove(TopWin, CursorY, CursorX);
      }

   return(CurrLine);
}
