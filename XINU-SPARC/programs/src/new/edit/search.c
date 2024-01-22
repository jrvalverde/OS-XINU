

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
 * Search(FirstLine, CurrLine, LastLine) -- search for the next repetition
 * of a given string.
 */

#include "common.h"

struct line *
Search(FirstLine, CurrLine, LastLine)

struct line *FirstLine, *CurrLine, *LastLine;

{
   char *tsb = Calloc(80);
   char *scwgetstr();
   struct line *GotoTopOfFile();
   extern struct line *TopLineOnScreen, *BotLineOnScreen;
   extern int CursorX, CursorY, LinePos, LineNum;
   char *c, *c2 = Calloc(160), *re_comp(), *re_exec();

   if (CurrLine == NULL) {
      free(srchstr);
      free(tsb);
      AnnounceError("No text to search.");
      return(CurrLine);
      }

   wclear(BotWin);
   
   wprintw(BotWin, "String for search: ");
   wrefresh(BotWin);
   srchstr = scwgetstr(BotWin);

   if (srchstr == NULL || *srchstr == NULLCHAR) 
      return(CurrLine);

   strcpy(c2, srchstr);
   for (c = c2; *c != NULLCHAR; c++) {
      switch (*c) {
         case '\\':
         case '.':
         case '^':
         case '$':
         case '[':
         case '*':  sprintf(tsb, "\\%s", c);
                    strcpy(c, tsb);
                    c++;
                    break;
         default:
                    break;
         }
      }

   free(tsb);

   c = Calloc(30);
   c = re_comp(c2);		/* set up the regex matcher */
   free(c2);

   if (c != NULL) {
      free(srchstr);
      AnnounceError("Can't understand the search string.");
      return(CurrLine);
      }
   else {
      char cc;

      wprintw(BotWin, "\nSearch Forward or Backward (f or b, f is default)? ");
      wrefresh(BotWin);

      cc = getchar();
      forward = !(cc == 'B' || cc == 'b');

      CurrLine = ActuallySearch(FirstLine, CurrLine, LastLine);
      return(CurrLine);

      }

}

/*
 * FindLeftMostOccurrence(s1, s2) -- returns the beginning position
 * of the leftmost occcurence of s2 in s1; returns -1 if not found.  
 * The return value is 1 less than the character number at which
 * the occurence starts, i.e. FLMO returns 4 if the match is found
 * starting at the 5th character.  This is to stay compatible
 * with C strings.
 */

FindLeftmostOccurrence(s1, s2)

char *s1, *s2;

{
   int i = 0, n2 = strlen(s2), n1 = strlen(s1);
   int found = FALSE;
   char *c = s1;
   
   while (! found && i <= n1 - n2) {
      if (found = !strncmp(c, s2, n2)) break;
      i++;
      c++;
      }

   return((found)? i : -1);

}

/*
 * FindRightMostOccurrence(s1, s2) -- returns the beginning position
 * of the rightmost occcurence of s2 in s1; returns -1 if not found.  
 * The return value is 1 less than the character number at which
 * the occurence starts, i.e. FRMO returns 4 if the match is found
 * starting at the 5th character.  This is to stay compatible
 * with C strings.
 */

FindRightmostOccurrence(s1, s2)

char *s1, *s2;

{
   int i = 0, n2 = strlen(s2), n1 = strlen(s1);
   int found = FALSE;
   char *c = s1;
   
   for (i = 0; i < n1 - n2; i++, c++);

   while (! found && i >= 0) {
      if (found = !strncmp(c, s2, n2)) break;
      i--;
      c--;
      }

   return((found)? i : -1);

}

/*
 *  ActuallySearch(first, curr, last) -- actually do the
 *  search.
 */

struct line *
ActuallySearch(FirstLine, CurrLine, LastLine)

struct line *FirstLine, *CurrLine, *LastLine;

{
      extern int LineNum;
      struct line *tptr;
      int tLineNum;
      int found = FALSE;

      for (tLineNum = LineNum, tptr = CurrLine; !found && tptr != NULL;) {
         int lp;
         found = ((re_exec(tptr->text)) > 0);
         if (!found)
              if (forward) {
                 tptr = tptr->next;
                 tLineNum++;
                 }
              else {
                 tptr = tptr->prev;
                 tLineNum--;
              }
         else
         if (tptr == CurrLine) {
            char *Tsb = Calloc(80 * sizeof(char));
               
            if (forward) {
               substr(Tsb, CurrLine->text, LinePos + 2);
               lp = FindLeftmostOccurrence(Tsb, srchstr) + 1;
               if (lp == 0) {
                  found = !found;
                  tptr = tptr->next;
                  tLineNum++;
                  }
               else
                  LinePos += lp;
               }
            else {
               strncpy(Tsb, CurrLine->text, LinePos);
               lp = FindRightmostOccurrence(Tsb, srchstr);
               if (lp == -1) {
                  found = !found;
                  tptr = tptr->prev;
                  tLineNum--;
                  }
               else
                  LinePos = lp;
  
               }
            }
         }

      if (found) {
         if (tptr != CurrLine) {
            if (tptr == FirstLine) {
               tptr = GotoTopOfFile();
               LinePos = FindRightmostOccurrence(tptr->text, srchstr);
               }
            else 
            if (tptr == LastLine) {
               tptr = GotoEndOfFile();
               LinePos = FindLeftmostOccurrence(tptr->text, srchstr);
               }
            else {
               LinePos = (forward) ? FindLeftmostOccurrence(tptr->text, srchstr)
                                 : FindRightmostOccurrence(tptr->text, srchstr);
               if (! IsOnScreen(tptr)) {
                     TopLineOnScreen = tptr;
                     BotLineOnScreen = FindBotLine(TopLineOnScreen);
                  }
               }
            }
         if (LinePos < 0) AnnounceError("Bad Line Position!!");
         LinePos = MAX(LinePos, 0);
         LineNum = tLineNum;
         CursorX = CountNChars(tptr->text, LinePos);
         UpdateScreen(TopLineOnScreen, BotLineOnScreen, tptr);
         return(tptr);
         }
      else {
         AnnounceError("String not found.");
         return(CurrLine);
         }
}
