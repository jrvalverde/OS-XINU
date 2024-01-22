

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
 * support.c -- support routines for the editor.
 *
 * 29 Sep 86
 *
 * sjc@cs.purdue.edu
 * Steve Chapin
 */

#include <curses.h>
#include <strings.h>
#include "ed.h"
#include "chars.h"
#include "fn.h"
#include "help.h"
#include <ctype.h>
#include <signal.h>

/*
 * Initialize() -- set up the windows, etc.
 */

Initialize()

{
	char *Calloc();
	extern Extra *UndoInfo;

	UndoInfo = (Extra *) Calloc(sizeof(Extra));
	Saved = TRUE;
	NumLines = LineNum = 1;

dbg("calling noecho\n");
	noecho();
dbg("calling crmode\n");
	crmode();
dbg("crmode completed\n");

	if (VB != NULL)
	    VBsb = (strlen(VB))? VB : "\007";
	else
	    VBsb = "\007";

	CursorY = CursorX = LinePos = 0;

dbg("calling newwin\n");
	TopWin = newwin(TOPWINLINES, 80, 0, 0);
dbg("calling newwin\n");
	HelpWin= newwin(TOPWINLINES, 80, 0, 0);
dbg("calling newwin\n");
	MidWin = newwin( 1,80,TOPWINLINES + 1,0);
dbg("calling newwin\n");
	StatWin= subwin( MidWin, 1, 10, TOPWINLINES + 1, 35);
dbg("calling newwin\n");
	BotWin = newwin( BOTWINLINES,80, TOPWINLINES + 2,0);
dbg("calling scrollok\n");
	scrollok(BotWin, TRUE);
	scrollok(TopWin, TRUE);
	wprintw(MidWin, "%s%s", "C----*---1---------2---------3---------4----"
		/*		 123456789 123456789 123456789 123456789 1234 */
                               , "-----5---------6---------7-+-------");
		/*		  56789 123456789 123456789 123456789 */
	wprintw(StatWin, "[loading]-");
dbg("calling wmove\n");
	wmove(TopWin, 0, 0);
dbg("calling wrefresh\n");
	wrefresh(MidWin);
dbg("calling wrefresh\n");
	wrefresh(StatWin);
dbg("calling wrefresh\n");
	wrefresh(BotWin);
dbg("calling wrefresh\n");
	wrefresh(TopWin);
	
dbg("calling initHelp\n");
	initHelp();
	
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	
}

/*
 * initHelp() -- intialize the help screen.
 */

initHelp()

{
	int i;
	extern char *mpihl[];

	wclear(HelpWin);
	for (i = 0; i < cHELPLINES; i++)
		wprintw(HelpWin, "%s\n", mpihl[i]);

	wprintw(HelpWin, "\n*'d commands may be prepended with a non-negative count.");
}

/*
 * LoadFile() -- read the file into memory.
 */

struct line *
LoadFile(clines)

int *clines;

{
	struct line *AllocLine();
	struct line *FLine, *tptr = AllocLine(), *tptr2;
	int i, j;
	char ttext[MAXLINE];
	int c;

        FLine = tptr;
	tptr2 = NULL;
	*clines = 0;

	while ((c = fgetc(InputFile)) != EOF) {
		i = 0;
		*clines = *clines + 1;
		ungetc(c, InputFile);
		while ((c = fgetc(InputFile)) != EOF && c != '\n') {
			
			if (c == '\t') {
			   int k, j = 8 - i % 8;
			   if (i + j >= MAXLINE) {
  			      ungetc('\n', InputFile);
			      }
                           else {
 			      for (k = 0; k < j; k++)
				 ttext[i+k] = ' ';
			      i += j;
                              }
                           }
                        else 
			if (++i >= MAXLINE-1) { 
				ttext[MAXLINE - 2] = c;
  			        if ((c = fgetc(InputFile)) != '\n') 
                                   ungetc(c, InputFile);
                                if (c != EOF) ungetc('\n', InputFile);
				}
			else {
				ttext[i-1] = c;
				}
			}

		ttext[i] = NULLCHAR;

		for (j = 0; j <=i; j++)
			tptr->text[j] = ttext[j];

		tptr->length = i;
		tptr->prev = tptr2;
		tptr2 = tptr;
		tptr->next = AllocLine();
		tptr = tptr->next;
		
		}

	if (tptr2 != NULL) 
 		tptr2 -> next =  NULL;

        if (*clines == 0) FLine = NULL;
	ChangeModeDisplay('c');
	return(FLine);
}

/*
 * AllocLine() -- allocate a struct line and init it.
 */

struct line *
AllocLine()

{
	struct line *tptr = (struct line *) Calloc (sizeof(struct line));
	if (tptr == NULL)
		panic("Out of memory in AllocLine().  Editor aborted.");
	return(tptr);
}

/*
 *  ShowFirstScreen() -- show the first screen
 */

ShowFirstScreen(FirstLine)

struct line *FirstLine;

{
	struct line *tptr;
	int i;

	BotLineOnScreen = TopLineOnScreen = FirstLine;

	for (tptr = FirstLine, i = 0;  i < TOPWINLINES && tptr != NULL;
			tptr = tptr -> next, i++) {
		wmove(TopWin, i, 0);
		scprintw(TopWin, tptr->text);
		BotLineOnScreen = tptr;
		}

	wmove(TopWin, 0, 0);
	wrefresh(TopWin);
}

/*
 * scprintw(Win, s) -- print the string, allowing for ctrl chars.
 */

scprintw(Win, s)

WINDOW *Win;
char *s;

{
	int i;

	for (i = 0; s[i] != 0; i++) 
		wprintw(Win, "%s", unctrl(s[i]));
}

/*
 * CopyLine(ptr1, ptr2)
 */

struct line *
CopyLine(ptr1)

struct line *ptr1;

{
	struct line *AllocLine();
	struct line *ptr2 = AllocLine();
	int i;

	ptr2->length = ptr1->length;
        ptr2->next   = ptr1->next;
        ptr2->prev   = ptr2->prev;
        for (i = 0; i < MAXLINE; i++)
           ptr2->text[i] = ptr1->text[i];

	return(ptr2);

}

/*
 * CopyLines(LinePtr) -- return a copy of all the lines pointed to
 * by LinePtr
 */

struct line *
CopyLines(LinePtr)

struct line *LinePtr;

{
	struct line *RetPtr, *tptr1, *tptr2;

	RetPtr = CopyLine(LinePtr);

 	for (tptr1 = RetPtr, tptr2 = LinePtr->next; tptr2 != NULL;
               tptr2 = tptr2->next, tptr1 = tptr1->next) {
		tptr1->next = CopyLine(tptr2);
		tptr1->next->prev = tptr1;
		}

	return(RetPtr);
}

/*
 * wgetstr(Win) read a string from the current position.
 */
 
char *
scwgetstr(Win)

WINDOW *Win;

{
	int c;
        int cChars = 0;
	char *sb2 = Calloc(80);
        char *sb1;
        int x, y;

	if (sb2 == NULL)
		panic("Out of memory in scwgetstr().  Editor aborted.");
	getyx(Win, y, x);

        sb1 = sb2;

	while ((c = wgetch(Win)) != '\n') {
		if (c == '\b') {
			if (sb1 != sb2) {
				*(--sb1) = NULLCHAR;
				wmove(Win, y,--x);
				wdelch(Win);
                                cChars--;
			}
		}
                else
                if (c == ESC) {
                   free(sb2);
                   return(NULL);
                }
		else 
                if (cChars < 79) {
                        cChars++;
      			*sb1 = c;
			c = isprint(c) ? c : '?';
   			waddch(Win, c);
                        sb1++;
			wmove(Win, y, ++x);
		}
		else
			beep();
	wrefresh(Win);
	}
	return(sb2);
}

/*
 * wgetnum -- returns a number...
 */

int
wgetnum(Win)

WINDOW *Win;

{
	return(atoi(scwgetstr(Win)));
}

/*
 * panic -- just quit!! something has gone wrong!
 */

panic(sb1)

char *sb1;

{
	clear();
	refresh();
	endwin();
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	printf("%s\n", sb1);
	exit (1);
}

/*
 * Calloc(size) -- allocate and clear the memory.
 */

char *
Calloc(size)

unsigned size;

{
   unsigned i = 1;
   char *c, *malloc();
   char *c1 = malloc(size);

   for (c = c1; i <= size; i++, c++)
      *c = 0;

   return(c1);
}

/*
 * CheckKeyCount (n) -- write out the file if CheckKeyCount mod
 * KEYMOD == 0;
 */

#define KEYMOD 50

int 
CheckKeyCount(n)

int n;

{ 
  int i;

   n++;
   if ((n % KEYMOD) == 0) {
	n = 0;
	i = ActuallySaveFile(sbTFile);
	if (i) {
		char *sb = Calloc(100);
		sprintf(sb, "Error writing file %s.  Please contact contest personnel.", sbTFile);
	AnnounceError(sb);
	}
   }
   return n;
}

/*
 * AnnounceError(s1, s2, s3, ...) -- clear the bottom window and
 * print the passed message.
 */

AnnounceError(s1)

char *s1;

{
   extern int ClearBotwin;

   ClearBotWin = TRUE;
   wclear(BotWin);
   wprintw(BotWin, s1);
   wrefresh(BotWin);
   beep();
   wmove(TopWin, CursorY, CursorX);
   wrefresh(TopWin);
}

/*
 * UpdateScreen() -- refresh the screen to reflect new contents.
 */

UpdateScreen(Top, Bot, Curr)

struct line *Top, *Bot, *Curr;

{
   struct line *tptr;
   int i = 0;
   extern int CursorY;

   wclear(TopWin);
   
   for (tptr = Top; tptr != Bot && tptr != NULL; tptr = tptr->next, i++) {
      wmove(TopWin, i, 0);
      CursorY = (Curr == tptr) ? i : CursorY;
      scprintw(TopWin, tptr->text);
      }

   if (tptr != NULL) {
      wmove(TopWin, i, 0);
      scprintw(TopWin, tptr -> text);
      }

   if (Curr == Bot) CursorY = i++;

   wmove(TopWin, CursorY, CursorX);

}

/*
 * GetYorN(s) -- return true for a yes response
 */

GetYorN(s)

char *s;

{
   char YorN;

   wprintw(BotWin, "%s ", s);
   wrefresh(BotWin);
   YorN = wgetch(BotWin);
   wprintw(BotWin, "%s", unctrl(YorN));
   return (YorN == 'y' || YorN == 'Y');

}

/*
 * ActuallySaveFile(sbFileName)
 */

int 
ActuallySaveFile(sbFileName)

char *sbFileName;

{
   FILE *OutputFile;
   extern struct line *FirstLine;
   struct line *FLine = FirstLine;
   int errflag;

   OutputFile = fopen(sbFileName, WRITEFILE);

   errflag = OutputFile == NULL;

   if (!errflag) {

      for (; FLine != NULL; FLine = FLine -> next)
         fprintf(OutputFile, "%s\n", FLine -> text);

      errflag = errflag || (fclose(OutputFile) == EOF);
   }

   return(errflag);

}

/*
 * IsLegalFileName(sbFile) -- check the filename against the
 * regular expression that is allowed for file names using
 * re_exec().
 */

IsLegalFileName(sbFile)

char *sbFile;

{
   char *sbErr;
   extern int Silent;
   char *re_comp();
   char *strcat();

   if (Silent && (! strcmp("../etc/question", sbFile))) {
      return(TRUE);
      }
   else {
/*      sbErr = Calloc(30);*/
/*      if (sbErr == NULL) */
/*         panic("Out of memory in IsLegalFileName.");*/

dbg("calling re_comp\n");
      sbErr = re_comp("^[_a-zA-Z0-9/][_a-zA-Z0-9\./]*$"); 

      if (sbErr != NULL) 
         panic("Unable to compile regular expression in IsLegalFileName.");

/*      free(sbErr);*/

dbg("calling re_exec\n");
      return(re_exec(sbFile));
      }
}

/*
 * GetLastLine(LinePtr) -- returns the last in a list of lines
 */

struct line *
GetLastLine(LinePtr)

struct line *LinePtr;

{
    if (LinePtr == NULL)
       return(NULL);

    for (; LinePtr->next != NULL; LinePtr = LinePtr->next);

    return(LinePtr);
}

/*
 * FindBotLine(TopLine) -- finds the bottom line marker given the
 * top line on the screen.
 */

struct line *
FindBotLine(TopLine)

struct line *TopLine;

{
   struct line *lptr;
   int cLines;

   if (TopLine == NULL) 
      return(NULL);

   for (lptr = TopLine, cLines = 1; lptr->next != NULL && cLines < TOPWINLINES;
               lptr = lptr->next, cLines++);

   return(lptr);
}

/*
 * FindTopLine(BotLine) -- finds the top line marker given the
 * bottom line on the screen.
 */

struct line *
FindTopLine(BotLine)

struct line *BotLine;

{
   struct line *lptr;
   int cLines;

   if (BotLine == NULL) 
      return(NULL);

   for (lptr = BotLine, cLines = 1; lptr->prev != NULL && cLines < TOPWINLINES;
               lptr = lptr->prev, cLines++);

   return(lptr);
}


/*
 *  CountLines(lptr) -- count the number of lines from lptr to
 *  a nil ptr.
 */

CountLines(lptr)

struct line *lptr;

{
   int i = 0;
   struct line *tptr;

   for (tptr = lptr; tptr != NULL; tptr = tptr->next)
      i++;

   return(i);
}

/*
 *  CleanUndoInfo(UndoInfo) -- free the space held by undo info...
 */

CleanUndoInfo(UndoInfo)

Extra *
UndoInfo;

{
   struct line *FreeLines();

   if (UndoInfo != NULL)
      switch(UndoInfo->tag) {
         case tagDELETELINE:
            UndoInfo->u.u_delline.prev = NULL;
            UndoInfo->u.u_delline.first =FreeLines(UndoInfo->u.u_delline.first);
            UndoInfo->u.u_delline.last = FreeLines(UndoInfo->u.u_delline.last);
            UndoInfo->tag = 0;
            break;
         case tagUNDODELETELINE:
            UndoInfo->u.u_delline.prev = NULL;
            UndoInfo->u.u_delline.first = NULL;
            UndoInfo->u.u_delline.last = NULL;
            UndoInfo->tag = 0;
            break;
         case tagINSERT:
            UndoInfo->tag = 0;
            UndoInfo->u.u_inschar.first = NULL;
            UndoInfo->u.u_inschar.last= NULL;
            UndoInfo->u.u_inschar.del = FreeLines(UndoInfo->u.u_inschar.del);
            UndoInfo->u.u_inschar.eLp = 0;
            UndoInfo->u.u_inschar.bLp = 0;
            break;
         case tagDELETECHAR:
            UndoInfo->u.u_delchar.first = NULL;
            UndoInfo->u.u_delchar.last = NULL;
            UndoInfo->u.u_delchar.del = FreeLines(UndoInfo->u.u_delchar.del);
            UndoInfo->tag = 0;
            UndoInfo->u.u_delchar.bLp = 0;
            UndoInfo->u.u_delchar.eLp = 0;
            break;
         case tagUNDODELETECHAR:
            UndoInfo->u.u_delchar.first = NULL;
            UndoInfo->u.u_delchar.last = NULL;
            UndoInfo->u.u_delchar.del = NULL;
            UndoInfo->tag = 0;
            UndoInfo->u.u_delchar.bLp = 0;
            UndoInfo->u.u_delchar.eLp = 0;
            break;
         default:
            break;
         }
      else
         panic("UndoInfo has been nullified.");
          
}

/*
 * FreeLines(lptr) -- free the lines pointed to by lptr.
 */

struct line *
FreeLines(lptr)

struct line *lptr;

{
   struct line *dummy;
   if (lptr != NULL) {
      dummy = FreeLines(lptr->next);
      free(lptr);
   }

   return(NULL);
}

/*
 * IsOnScreen(lptr) -- check if a line is on the screen
 */

IsOnScreen(lptr)

struct line *lptr;

{
   extern struct line *TopLineOnScreen, *BotLineOnScreen;
   struct line *tptr;
   int i = FALSE;

   for (tptr = TopLineOnScreen; tptr != NULL && tptr != BotLineOnScreen;
         tptr = tptr->next)
      if (tptr == lptr) i = TRUE;

   if (lptr == BotLineOnScreen && lptr != NULL) i = TRUE;

   return(i);
}

/*
 * ChangeMode(c) -- change the current displayed mode.
 */

ChangeModeDisplay(c)

char c;

{
   switch (c) {
	case	'i': 	wclear(StatWin);
			wprintw(StatWin, "[insert]--");
		     	wrefresh(StatWin);
			wrefresh(TopWin);
			break;
	case	'c': 	wclear(StatWin);
			wprintw(StatWin, "[command]-");
		     	wrefresh(StatWin);
			wrefresh(TopWin);
			break;
	default:	break;
	}
}

/*
 * substring(s1, s2, n) -- return in s1 the substring of s2 starting
 * at the nth char
 */

char *
substr(s1, s2, n)

char *s1, *s2;
int n;

{
   char *c;
   int i;

   for (c = s2, i = 1; i < n && *c != NULLCHAR; c++, i++);

   strcpy(s1, c);
}

/*
 * CountNChars(sb, n) -- count the number of cursor positions
 * in the first n characters of the string sb.
 */

int
CountNChars(sb, n)

char *sb;
int n;

{
   int i, j = 0;
   char *c;

   for (i = 1, c = sb; *c != NULLCHAR && i <= n; i++, c++)
      j += strlen(unctrl(*c));

   return(j);
}
