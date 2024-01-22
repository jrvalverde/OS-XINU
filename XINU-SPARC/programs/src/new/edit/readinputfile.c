

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
 * ReadInputFile(CurrentLine) -- read in a file after the current line.
 */

#include "common.h"

struct line *
ReadInputFile(CurrentLine)

struct line *CurrentLine;

{
   extern struct line *FirstLine, *LastLine, *TopLineOnScreen, *BotLineOnScreen;
   extern int NumLines;
   extern Extra *UndoInfo;
   extern char *index();
   extern FILE *InputFile;
   char *FileName;
   struct line *FLine, *LLine;
   extern int Saved;
   int SaveN;
   
   wclear(BotWin);
   wprintw(BotWin, "Name of file to be read: ");
   wrefresh(BotWin);
   FileName = scwgetstr(BotWin);
   if (FileName == NULL || *FileName == NULLCHAR) return(CurrentLine);
     
   if (! IsLegalFileName(FileName)) {
      /* gotcha, you dirty rotten little Unix (tm) hacker... */
      AnnounceError("Illegal file name.");
      }
   else {  /* assume we've got a "legal" filename */
      if ((InputFile = fopen(FileName, READFILE)) == NULL) {
         AnnounceError("Cannot open file ");
         wprintw(BotWin, "'%s'.", FileName);
         wrefresh(BotWin);
         }
      else {
         Saved = FALSE;
         FLine = LoadFile(&SaveN);
         LLine = GetLastLine(FLine);

         wclear(BotWin);
         wprintw(BotWin, "File '%s' read.", FileName);
         wrefresh(BotWin);

         CleanUndoInfo(UndoInfo);
         UndoInfo->tag = tagINPUT;
         UndoInfo->u.u_delline.prev = CurrentLine;
         UndoInfo->u.u_delline.first = FLine;
         UndoInfo->u.u_delline.last = LLine; 

         if (CurrentLine == NULL) {
            FirstLine = TopLineOnScreen = FLine;
            LastLine = LLine;
            NumLines = SaveN;
            BotLineOnScreen = FindBotLine(TopLineOnScreen);
            UpdateScreen(TopLineOnScreen, BotLineOnScreen, FLine);
            return(FLine);
            }
         else {
            if (CurrentLine != LastLine && LLine != NULL) 
               CurrentLine->next->prev = LLine;
            if (LLine != NULL)
               LLine->next = CurrentLine->next;
            if (FLine != NULL) {
               FLine->prev = CurrentLine;
               CurrentLine->next = FLine;
               }
            if (CurrentLine != BotLineOnScreen || BotLineOnScreen == LastLine) {
               BotLineOnScreen = FindBotLine(TopLineOnScreen);
               UpdateScreen(TopLineOnScreen, BotLineOnScreen, CurrentLine);
               }
            if (CurrentLine == LastLine)
               LastLine = LLine;
            NumLines += SaveN;
            }
         }
      }
   return(CurrentLine);   
}
