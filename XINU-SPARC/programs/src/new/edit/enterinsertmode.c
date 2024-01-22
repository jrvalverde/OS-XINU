

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
 * EnterInsertMode() -- go into insert mode and insert stuff, then
 * return the current line pointer (may be different than the original)
 * Thus far, this is only called from GetCommand, so no UserCall
 * flag has been put in.
 */

#include "common.h"
#include "chars.h"

struct line *
EnterInsertMode(CurrLine)

struct line *CurrLine;

{
   char c;
   int i, j;
   extern int cKeystrokes, LinePos;
   int cChars = 0, cMaxNegChars = 0;
   extern Extra *UndoInfo;
   struct line *TempLine2, *TempLine = CurrLine;
   struct line *AllocLine();
   struct line *TempDelText = AllocLine();
   extern struct line *TopLineOnScreen, *BotLineOnScreen;
   int WasEsc = FALSE;

#ifdef DEBUG
   extern FILE *Logfile;
#endif 

   ChangeModeDisplay('i');

   if (TempLine == NULL)   /* should only be with an empty file... */
        TopLineOnScreen = BotLineOnScreen = FirstLine = LastLine = TempLine 
           = AllocLine();

   wclear(BotWin);
   wprintw(BotWin, "%s\n", "In insert mode.");
   wprintw(BotWin, "Hit ESC to exit.");
   wrefresh(BotWin);
   wrefresh(TopWin);

   if ((c = wgetch(TopWin)) != ESC) {
      CleanUndoInfo(UndoInfo);
      UndoInfo->tag = tagINSERT;
      UndoInfo->u.u_inschar.first = TempLine;
      UndoInfo->u.u_inschar.last = TempLine;
      UndoInfo->u.u_inschar.bLp = LinePos;
      UndoInfo->u.u_inschar.eLp = LinePos;
      }
   else
      WasEsc = TRUE;

   ungetc(c, stdin);

   while ((c = wgetch(TopWin)) != ESC) {
   
#ifdef DEBUG
      if (c != '\n')
         fprintf(Logfile, "%s", unctrl(c));
      else
         fprintf(Logfile, "\n");
#endif

      cKeystrokes = CheckKeyCount(cKeystrokes);
      if (c == '\n') {
         cChars++;

         wclrtoeol(TopWin);   /* clear to end of line */
         TempLine2 = AllocLine();
         TempLine2->next = TempLine->next;
         TempLine2->prev = TempLine;
         TempLine->next = TempLine2;
         if (TempLine2->next != NULL)
            TempLine2->next->prev = TempLine2;

         NumLines++;
         LineNum++;

         j = TempLine->length;
         for (i = LinePos; i < j; i++) {
            TempLine2->text[i - LinePos] = TempLine->text[i];
            TempLine->length--;
            TempLine->text[i] = 0;
            TempLine2->length++;
            }

         for (i = LinePos; i < MAXLINE; i++)  /* clear out any old text... */
            TempLine->text[i] = 0;

         if (TempLine == LastLine || TempLine == BotLineOnScreen) {
               waddch(TopWin, '\n');
               if (TempLine == LastLine) LastLine = TempLine2;
               if (TempLine == BotLineOnScreen) {
                  BotLineOnScreen = TempLine2;
                  if (CursorY == (TOPWINLINES - 1))
                     TopLineOnScreen = TopLineOnScreen -> next;
                  }
               if (CursorY < (TOPWINLINES - 1)) CursorY++;
               if (! wmove(TopWin, CursorY, 0)) AnnounceError("Bad Move.");
               wclrtoeol(TopWin);
               scprintw(TopWin, TempLine2->text);
               }
            else {      /* just an average line */
               int cLines;
               struct line *Lptr;

               CursorY++;
               wmove(TopWin, CursorY, 0);
               winsertln(TopWin);
               wmove(TopWin, CursorY, 0);
               scprintw(TopWin, TempLine2->text);
               wmove(TopWin, CursorY, 0);

               for (Lptr = TopLineOnScreen, cLines = 1; 
                  Lptr->next != NULL && cLines < TOPWINLINES;
                     Lptr = Lptr->next, cLines++);
               BotLineOnScreen = Lptr;
            }
      LinePos = CursorX = 0;
      UndoInfo->u.u_inschar.eLp = 0;
      UndoInfo->u.u_inschar.last = TempLine = TempLine2;
      }
      else {
        if (c == '\b') {
            if (LinePos == 0) {
               if (TempLine == FirstLine)
                  AnnounceError("At beginning of file.");
               else {
                  if(TempLine->length + TempLine->prev->length >= MAXLINE) 
                     AnnounceError("Joining lines would result in an overly long line.");
                  else {
                     UndoInfo->u.u_inschar.eLp = 
                     UndoInfo->u.u_inschar.bLp = TempLine->prev->length;
                     UndoInfo->u.u_inschar.last = 
                           UndoInfo->u.u_inschar.last->prev;

                     if (--cChars < cMaxNegChars) {
                        cMaxNegChars = cChars;
                        UndoInfo->u.u_inschar.first =
                           UndoInfo->u.u_inschar.first->prev;
                        TempDelText->prev = AllocLine();
                        TempDelText->prev->next = TempDelText;
                        TempDelText = TempDelText->prev;
                        }
                        
                     TempLine = MoveCharUp(TempLine);
                     TempLine = MeldLines(TempLine);
                     }
                  }
               }
            else if (LinePos > 0) {
               char *Tsb = Calloc(80 * sizeof(char));
               
               if (--cChars < cMaxNegChars) {
                  sprintf(Tsb, "%c%s", TempLine->text[LinePos-1], 
                     TempDelText->text);
                  strcpy(TempDelText->text, Tsb);
                  TempDelText->length++;
                  UndoInfo->u.u_inschar.bLp--;
                  cMaxNegChars = cChars;
                  }

               CursorX = MAX(0, CursorX - 1);
               LinePos = MAX(0, LinePos - 1);
               wmove(TopWin, CursorY, CursorX);
               if (strlen(unctrl(TempLine->text[LinePos])) == 2) {
                  wmove(TopWin, CursorY, --CursorX);
                  wdelch(TopWin);
                  }
               UndoInfo->u.u_inschar.eLp--;
               wdelch(TopWin);
               for (i = LinePos; i < TempLine->length; i++)
                  TempLine->text[i] = TempLine->text[i+1];
               TempLine->length = MAX(0, TempLine->length - 1);
            }
         else
            panic("Bad line position in EnterInsertMode()");
         }  
         else
         if (TempLine -> length + strlen(unctrl(c)) >= MAXLINE) 
            AnnounceError("Line will be too long!");
         else 
            if (c == '\t') {
               int j = 8 - LinePos % 8;

               if (TempLine->length >= MAXLINE - j)
                  AnnounceError("Line will be too long!");
               else {
                  for (i = TempLine->length - 1; i >= LinePos; i--)
                     TempLine->text[i+j] = TempLine->text[i];
                  for (i = 0; i < j; i++) {
                     TempLine->text[LinePos + i] = ' ';
                     winsch(TopWin, ' ');
                     }
                  CursorX += j;
                  LinePos += j;
                  TempLine->length += j;
                  UndoInfo->u.u_inschar.eLp += j;
                  cChars += j;
                  wmove(TopWin, CursorY, CursorX);
               }
            }
            else {
               char *sb;

               for (i = TempLine->length - 1; i >= LinePos; i--)
                  TempLine->text[i+1] = TempLine->text[i];

               TempLine->text[LinePos] = c;
               sb = unctrl(c);

               CursorX += strlen(sb);
               LinePos++;

               for (i = 0; i < strlen(sb); i++)
                  winsch(TopWin, ' ');
               
               scprintw(TopWin, sb);
               TempLine->length += 1;
               UndoInfo->u.u_inschar.eLp++;
               cChars++;
               wmove(TopWin, CursorY, CursorX);
         }
      }
      wmove(TopWin, CursorY, CursorX);
      wrefresh(TopWin);
   }

   if (! WasEsc)
      UndoInfo->u.u_inschar.del = TempDelText;

#ifdef DEBUG
   fprintf(Logfile, "\n%s\n", unctrl(c));
   fflush(Logfile);
#endif

   wclear(BotWin);
   wrefresh(BotWin);

   ChangeModeDisplay('c');
   return(TempLine);

}
