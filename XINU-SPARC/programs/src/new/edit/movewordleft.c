

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
 * MoveWordLeft(CurrLine) -- move a word left.
 */

#include "common.h"

struct line *
MoveWordLeft(CurrLine)

struct line *CurrLine;

{
   struct line *MoveCharLeft();
   struct line *MoveCharRight();
   extern int BadMove;

   CurrLine = MoveCharLeft(CurrLine);
   if (BadMove) return(CurrLine);

   while ((! (isalpha(CurrLine->text[LinePos]) || 
              isdigit(CurrLine->text[LinePos]))) && ((CurrLine != FirstLine)
              ||  (LinePos != 0))) 
      CurrLine = MoveCharLeft(CurrLine);

   while (((isalpha(CurrLine->text[LinePos]) || 
            isdigit(CurrLine->text[LinePos]))) && ((CurrLine != FirstLine)
               || (LinePos != 0))) 
      CurrLine = MoveCharLeft(CurrLine);

   if (CurrLine != FirstLine || LinePos != 0)
      return(MoveCharRight(CurrLine));
   else
      return(CurrLine);
}
