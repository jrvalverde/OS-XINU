

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
 * UndoLastDeletion() -- replace the deleted line
 */

#include "common.h"

struct line *
UndoLastDeletion(UndoInfo)

Extra *UndoInfo;

{
   extern struct line *CurrLine;
   extern struct line *FirstLine, *LastLine;
   struct line *Lptr = CurrLine;
   extern int NumLines, LinePos, LineNum, CursorX, CursorY;
   struct line *tptr1, *tptr2, *tptr3;
   int cLines;
   extern int ClearBotWin;

   switch (UndoInfo->tag) {
      case tagDELETELINE: 
      case tagUNDOPUT:
      case tagUNDOINPUT:
         LinePos = CursorX = 0;
         tptr1 = UndoInfo->u.u_delline.prev;
         tptr2 = UndoInfo->u.u_delline.first;
         tptr3 = UndoInfo->u.u_delline.last;
	 cLines = CountLines(tptr2);
         if (!IsOnScreen(tptr1))
            TopLineOnScreen = (tptr1 != NULL) ? tptr1 : tptr2;
         if (tptr1 != NULL) {
            tptr3->next = tptr1->next;
            tptr1->next = tptr2;
            }
         else {
            tptr3->next = FirstLine;
            FirstLine = tptr2;
            }
         tptr2->prev = tptr1;

         if (tptr3->next != NULL)
            tptr3->next->prev = tptr3;

         Lptr = (UndoInfo->tag == tagDELETELINE || tptr1 == NULL) 
                           ? tptr2 : tptr1;
         UndoInfo->tag = (UndoInfo->tag == tagUNDOPUT) ? tagPUT : 
            (UndoInfo->tag == tagUNDOINPUT) ? tagINPUT : 
                           tagUNDODELETELINE;
	 wclear(BotWin);
	 if (cLines == 1)
	    wprintw(BotWin, "1 line of text added.");
	 else
	    wprintw(BotWin, "%d lines of text added.", cLines);
	 wrefresh(BotWin);
         ClearBotWin = TRUE;
         break;
      case tagDELETECHAR: 
         UndoInfo -> tag = tagUNDODELETECHAR;
         tptr1 = UndoInfo->u.u_delchar.first;
         tptr2 = UndoInfo->u.u_delchar.del;
         if (tptr2->next == NULL) {
            char *Tsb1 = Calloc(80 * sizeof(char));
            char *Tsb2 = Calloc(80 * sizeof(char));
            int i, j = UndoInfo->u.u_delchar.bLp;

            UndoInfo->u.u_delchar.eLp = j + tptr2->length;
            tptr3 = tptr1;
            Tsb1 = strncpy(Tsb1, tptr1->text, j);
            Tsb1 = strcat(Tsb1, tptr2->text);
            for (i = 0; i <= tptr1->length - j; i++)
               Tsb2[i] = tptr1->text[i + j];
            Tsb1 = strcat(Tsb1, Tsb2);
            strcpy(tptr1->text, Tsb1);
            tptr1->length = strlen(tptr1->text);
            
            free(Tsb1);
            free(Tsb2);
            }
         else {
            int j = UndoInfo->u.u_delchar.bLp;
            struct line *tptr4 = GetLastLine(tptr2);

            tptr3 = AllocLine();
            tptr3->length = tptr1->length - j;
            substr(tptr3->text, tptr1->text, j+1);
            tptr1->text[j] = NULLCHAR;
            strcat(tptr1->text, tptr2->text);
            UndoInfo->u.u_delchar.eLp = tptr4->length;
            if (tptr2->next == tptr4) {
               tptr3->prev = tptr1;
               tptr3->next = tptr1->next;
               tptr1->next = tptr3;
               if (tptr3->next != NULL)
                  tptr3->next->prev = tptr3;
               }
            else {
               tptr3->next = tptr1->next;
               if (tptr3->next != NULL)
                  tptr3->next->prev = tptr3;
               tptr1->next = tptr2->next;
               tptr1->next->prev = tptr1;
               tptr3->prev = tptr4->prev;
               tptr3->prev->next = tptr3;
               }
            strcat(tptr4->text, tptr3->text);
            strcpy(tptr3->text, tptr4->text);
            tptr3->length = strlen(tptr3->text);
            tptr1->length = strlen(tptr1->text);

            free(tptr2); free(tptr4);
            }
         Lptr = tptr1;
         if (! IsOnScreen(Lptr))
            TopLineOnScreen = Lptr;
         LinePos = UndoInfo->u.u_delchar.bLp;
         CursorX = CountNChars(tptr1->text, LinePos);
         UndoInfo->u.u_delchar.last = tptr3;
         break;
      case tagUNDODELETELINE: 
      case tagPUT:
      case tagINPUT:
         LinePos = CursorX = 0;
         tptr1 = UndoInfo->u.u_delline.prev;
         tptr2 = UndoInfo->u.u_delline.first;
         tptr3 = UndoInfo->u.u_delline.last;
         if (!IsOnScreen(tptr1))
            TopLineOnScreen = tptr1;
         if (tptr1 != NULL) {
            Lptr = (UndoInfo->tag == tagUNDODELETELINE && 
                            tptr3->next != NULL) ? tptr3->next : tptr1;
            tptr1->next = tptr3->next;
            }
         else 
            Lptr = TopLineOnScreen = FirstLine = tptr3->next;
         tptr2->prev = NULL;
         if (tptr3->next != NULL) {
            tptr3->next->prev = tptr1;
            }
         else {
            LastLine = Lptr = tptr1;
            }
         tptr3->next = NULL;
	 wclear(BotWin);
	 cLines = CountLines(tptr2);
	 if (cLines == 1)
  	    wprintw(BotWin, "1 line of text deleted.");
	 else
	    wprintw(BotWin, "%d lines of text deleted.", cLines);
	 wrefresh(BotWin);
         ClearBotWin = TRUE;
         UndoInfo->tag = (UndoInfo->tag == tagPUT) ? tagUNDOPUT : 
            (UndoInfo->tag == tagINPUT) ? tagUNDOINPUT : tagDELETELINE;
         break;
      case tagUNDODELETECHAR: 
         tptr1 = UndoInfo->u.u_delchar.first;
         tptr2 = UndoInfo->u.u_delchar.last;
         tptr3 = AllocLine();

         if (tptr1 == tptr2) {
            char *Tsb = Calloc(80 * sizeof(char));

            substr(Tsb, tptr1->text, UndoInfo->u.u_delchar.eLp+1);
            substr(tptr3->text, tptr1->text, UndoInfo->u.u_delchar.bLp+1);
            tptr3->text[strlen(tptr3->text) - strlen(Tsb)] = NULLCHAR;
            tptr1->text[UndoInfo->u.u_delchar.bLp] = NULLCHAR;
            strcat(tptr1->text, Tsb);
            tptr1->length = strlen(tptr1->text);
            tptr3->length = strlen(tptr3->text);

            UndoInfo->u.u_delchar.del = tptr3;
            CursorX = CountNChars(tptr1->text, UndoInfo->u.u_delchar.bLp);
            
            }
         else {
            struct line *tptr4 = AllocLine();

            substr(tptr4->text, tptr2->text, UndoInfo->u.u_delchar.eLp+1);
            tptr2->text[UndoInfo->u.u_delchar.eLp] = NULLCHAR;
            tptr2->length = strlen(tptr2->text);
            substr(tptr3->text, tptr1->text, UndoInfo->u.u_delchar.bLp+1);
            tptr1->text[UndoInfo->u.u_delchar.bLp] = NULLCHAR;
            strcat(tptr1->text, tptr4->text);
            tptr1->length = strlen(tptr1->text);
            tptr3->next = tptr1->next;
            tptr3->prev = NULL;
            tptr1->next = tptr2->next;
            tptr3->next->prev = tptr3;
            if (tptr1->next != NULL)
               tptr1->next->prev = tptr1;
            tptr2->next = NULL;
            UndoInfo->u.u_delchar.del = tptr3;
            }
         UndoInfo->tag = tagDELETECHAR;
         LinePos = UndoInfo->u.u_delchar.eLp = UndoInfo->u.u_delchar.bLp; 
         Lptr = tptr1;
         if (!IsOnScreen(Lptr))
            TopLineOnScreen = Lptr;
         CursorX = CountNChars(tptr1->text, LinePos);
         break;
      case tagINSERT: {
         struct line *tptr4;
         int b = UndoInfo->u.u_inschar.bLp;
         int e = UndoInfo->u.u_inschar.eLp;
         struct line *tptr5;
 
         tptr1 = UndoInfo->u.u_inschar.first;
         tptr2 = UndoInfo->u.u_inschar.last;
         tptr3 = UndoInfo->u.u_inschar.del;
         tptr4 = GetLastLine(tptr3);

         /* first, delete the inserted text... */
         if (tptr1 == tptr2) {
            if (tptr1 != NULL) {
               char *Tsb = Calloc(80 * sizeof(char));

               tptr5 = AllocLine();
               substr(tptr5->text, tptr1->text, b + 1);
               substr(Tsb, tptr2->text, e + 1);
               tptr5->text[e - b] = NULLCHAR;
               tptr5->length = e - b;
               tptr1->text[b] = NULLCHAR;
               strcat(tptr1->text, Tsb); 
               tptr1->length = strlen(tptr1);

               free(Tsb);
               }
            }
         else {
            char *Tsb = Calloc(80 * sizeof(char));

            tptr5 = CopyLine(tptr1);
            tptr5->next->prev = tptr5;
            tptr5->prev = NULL;
            substr(Tsb, tptr2->text, e + 1);
            tptr1->text[b] = NULLCHAR;
            strcat(tptr1->text, Tsb);
            tptr2->text[e] = NULLCHAR;
            tptr1->next = tptr2->next;
            if (tptr1->next != NULL)
               tptr1->next->prev = tptr1;
            substr(Tsb, tptr5->text, b + 1);
            strcpy(tptr5->text, Tsb);
            tptr5->length = strlen(tptr5);
            tptr2->next = NULL;

            free(Tsb);
            }

         /* now re-insert the deleted text... */
         if (tptr3 == tptr4) {
            if (tptr3 != NULL) {	/* only 1 line */
               char *Tsb = Calloc(80 * sizeof(char));

               UndoInfo->u.u_inschar.eLp = tptr3->length + b;
               substr(Tsb, tptr1->text, b+1);
               tptr1->text[b] = NULLCHAR;
               strcat(tptr1->text, tptr3->text);
               strcat(tptr1->text, Tsb);
               tptr1->length = strlen(tptr1->text);

               free(Tsb);
               tptr3 = FreeLines(tptr3);
               }
            else {
               UndoInfo->u.u_inschar.eLp = b;
               }
            UndoInfo->u.u_inschar.last = tptr1;
            Lptr = tptr1;
            }
         else {
            char *Tsb = Calloc(80 * sizeof(char));
            struct line *tptr4 = GetLastLine(tptr3);

            substr(Tsb, tptr1->text, b+1);
            UndoInfo->u.u_inschar.eLp = strlen(tptr4->text);
            tptr1->text[b] = NULLCHAR;
            strcat(tptr1->text, tptr3->text);
            tptr1->length = strlen(tptr1->text);
            strcat(tptr4->text, Tsb);
            tptr4->next = tptr1->next;
            if (tptr4->next != NULL)
               tptr4->next->prev = tptr4;
            tptr1->next = tptr3->next;
            tptr1->next->prev = tptr1;

            UndoInfo->u.u_inschar.last = tptr4;
            Lptr = tptr4;
            tptr3->next = NULL;
            tptr3 = FreeLines(tptr3);
            free(Tsb);
            }
         if (! IsOnScreen(Lptr))
            TopLineOnScreen = Lptr;
         LinePos = UndoInfo->u.u_delchar.eLp;
         CursorX = CountNChars(Lptr->text, LinePos);
         UndoInfo->u.u_inschar.del = tptr5;
         }
         break;
      case NULLCHAR : 
         break;
      default: panic("Illegal tag field in UndoLastDeletion.");
         break;
      }

   NumLines = CountLines(FirstLine);
   LineNum = NumLines - CountLines(Lptr);
   if (FirstLine != NULL) LineNum++;
   LastLine = GetLastLine(FirstLine);
   BotLineOnScreen = FindBotLine(TopLineOnScreen);
   UpdateScreen(TopLineOnScreen, BotLineOnScreen, Lptr);
   return(Lptr);
}
