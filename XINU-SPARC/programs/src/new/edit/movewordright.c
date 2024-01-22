

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
 * MoveWordRight(CurrLine) -- move a word right.
 */

#include "common.h"

struct line *
MoveWordRight(CurrLine)

struct line *CurrLine;

{
   struct line *MoveCharLeft();
   struct line *MoveCharRight();
   extern int BadMove;

   if (CurrLine == LastLine && LinePos == CurrLine->length) {
      AnnounceError("At end of file.  Can't advance.");
      BadMove = TRUE;
      return(CurrLine);
      }

   while (((isalpha(CurrLine->text[LinePos]) || 
            isdigit(CurrLine->text[LinePos]))) && ((CurrLine != LastLine)
               || (LinePos != LastLine->length))) 
      CurrLine = MoveCharRight(CurrLine);

   while ((! (isalpha(CurrLine->text[LinePos]) ||
              isdigit(CurrLine->text[LinePos]))) && ((CurrLine != LastLine)
               || (LinePos != LastLine->length))) 
      CurrLine = MoveCharRight(CurrLine);

   return(CurrLine);
}
