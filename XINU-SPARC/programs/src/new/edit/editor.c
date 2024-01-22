

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
 * editor.c -- the mainline and command grabber for the acm contest
 * editor.
 *
 * 27 Jun 86
 *
 * sjc@cs.purdue.edu
 * Steve Chapin
 */

#include <curses.h>
#include "ed.h"
#include "chars.h"
#include <ctype.h>
#include <sgtty.h>
#include "fn.h"

#include <tty.h>


int oldstdinbufsiz;
int usewyse50 = 0;
extern char libwarning;

/*
 *  main -- the mainline for the editor.
 */

main(argc, argv)
int argc;
char *argv[];
{
	extern int cKeystrokes;
	extern int Silent;
	extern Extra *UndoInfo;
	int pid, dummy;
	char *strcat();
	int i;

#ifdef DEBUG
        extern FILE *Logfile;
#endif

	libwarning = 0;
	
dbg("editor has started\n");
dbg("argc = %d\n", argc);
dbg("argv[0] = %s\n", argv[0]);
dbg("argv[1] = %s\n", argv[1]);


	if (argc == 1) {
		printf("Usage: edit [-d] [-w] filename \n");
		exit(0);
	}

	for (i=1; i<argc; i++) {
		if ((strcmp(argv[i], "-s")) == 0) {
			Silent = TRUE;
			FileName = argv[2];
		}
		else if ((strcmp(argv[i], "-w")) == 0) {
			termwyse50();
		}
		else if ((strcmp(argv[i], "-d")) == 0) {
			libwarning = 1;
		}
		else if ((i+1) == argc) {
			FileName = argv[i];
dbg("Filename = %s\n", FileName);
		}
	}

dbg("Now checking if legal file name\n");
#ifdef CHECK_FILE_NAME
	if (! IsLegalFileName(FileName)) {
		printf("Error: %s\n%s\n%s\n",
		   FileName,
	   	   "Illegal file name.  File names may be composed of letters,",
		   "digits, and periods but may not begin with a period.");
  		exit(1);
	}
#endif
dbg("Must be a legal file name\n");

dbg("Calling initscr\n");
        initscr();

dbg("Trying to set the input to work correctly\n");
	xcontrol(0, TCMODEK);
	xcontrol(0, TCNOECHO);
	stdin->_flag = stdin->_flag | _IONBF;

dbg("Calling Initialize\n");
	Initialize();
	
dbg("Now opening file %s\n", FileName);
	if ((InputFile = fopen(FileName, READFILE)) == NULL) {
		if (! Silent) {
			wclear(BotWin);
			wprintw(BotWin,"Creating file %s\n", FileName);
			wrefresh(BotWin);
		}
		FirstLine = CurrLine = LastLine = NULL;
		}
	else {
		CurrLine = FirstLine = LoadFile(&NumLines);
                LastLine = GetLastLine(FirstLine);
		fclose(InputFile);
		}

dbg("Calling ShowFirstScreen\n");
	ShowFirstScreen(FirstLine);

	wprintw(BotWin, "Type a ? for help.");
	wrefresh(BotWin);
	wrefresh(TopWin);
	
	pid = getpid();
        sbTFile = Calloc(50);
        if (sbTFile == NULL)
		panic("Out of memory at initialization.  Editor Aborted.");
	sprintf(sbTFile, "/usr/tmp/acmedit%d", pid);
        sbLFile = Calloc(50);
        if (sbLFile == NULL)
		panic("Out of memory at initialization.  Editor Aborted.");
#ifdef DEBUG
	sprintf(sbLFile, "/usr/tmp/acmlog%d", pid);
        Logfile = fopen(sbLFile, WRITEFILE);
        if (Logfile == NULL) {
           panic("Can't open the log file.");
        } 
#endif

	cKeystrokes = 0;


	ActuallySaveFile(sbTFile);

	for (;;) {
		GetCommand();
	}

}

/*
 *  GetCommand.c -- get the command from stdin and then call
 *  the appropriate routine to perfrom the action.
 */

GetCommand()

{
	char command = getchar();

#ifdef DEBUG
        extern FILE *Logfile;
#endif

	int i;
	extern int forward;
	extern int BadMove, ClearBotWin;

#ifdef DEBUG
	fprintf(Logfile, "%s\n", unctrl(command));
        fflush(Logfile); 
#endif

	if (ClearBotWin) {
		wclear(BotWin);
		wrefresh(BotWin);
		ClearBotWin = FALSE;
		}

	if (isdigit(command)) {
		ClearBotWin = TRUE;
		wclear(BotWin);
		cCommand = command - '0';
		wprintw(BotWin, "Command count: %d", cCommand);
		wrefresh(BotWin);
		wrefresh(TopWin);
		while (isdigit(command = getchar())) {
			cCommand = cCommand * 10 + command - '0';
			wprintw(BotWin, "%c", command);
			wrefresh(BotWin);
			wrefresh(TopWin);
		}
		ungetc(command, stdin);
	} else {
		cCommand = MAX(cCommand, 1);
		switch (command) {
			case BEGOFLINE:
				GotoBegOfLine();
				break;
			case CHARLEFT: 
			case '\b'    :
				for (i = 1; i <= cCommand && !BadMove; i++) 
					CurrLine = MoveCharLeft(CurrLine);
				break;
			case WORDLEFT:
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = MoveWordLeft(CurrLine);
				break;
			case CHARRIGHT: 
			case ''     :
			case ' '     :
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = MoveCharRight(CurrLine);
				break;
			case WORDRIGHT:
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = MoveWordRight(CurrLine);
				break;
			case CHARUP: 
			case ''  :
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = MoveCharUp(CurrLine);
				break;
			case CHARDOWN:
			case '\n'    :
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = MoveCharDown(CurrLine);
				break;
			case TOPOFFILE:
				CurrLine = GotoTopOfFile();
				break;
			case ENDOFFILE:
				CurrLine = GotoEndOfFile();
				GotoEndOfLine(CurrLine);
				break;
			case ENDOFLINE:
				GotoEndOfLine(CurrLine);
				break;
			case DELETECHAR:
				Saved = FALSE;
				CurrLine = DeleteChar(CurrLine, cCommand,TRUE);
				cKeystrokes = CheckKeyCount(cKeystrokes);
				break;
			case DELETELINE:
				Saved = FALSE;
				CurrLine = DeleteLine(CurrLine,cCommand,TRUE);
				cKeystrokes = CheckKeyCount(cKeystrokes);
				break;
			case REDRAW:
				clear();
				refresh();
				touchwin(MidWin);
				wrefresh(MidWin);
				touchwin(StatWin);
				wrefresh(StatWin);
				touchwin(BotWin);
				wrefresh(BotWin);
				touchwin(TopWin);
				break;
			case STATUS:
				ShowStatus();
				break;
			case UNDODEL:
				CurrLine = UndoLastDeletion(UndoInfo);
				break;
			case YANK:
				Yank(cCommand);
				break;
			case PUT:
				CurrLine = PutAfter(CurrLine, cCommand);
				cKeystrokes = CheckKeyCount(cKeystrokes);
				break;
			case WRITE:
				SaveFile(FileName);
				break;
			case QUIT:
				Quit();
				break;
			case INSERTCHAR:
				Saved = FALSE;
				CurrLine = EnterInsertMode(CurrLine);
				break;
			case INPUTFILE:
				CurrLine = ReadInputFile(CurrLine);
				break;
			case GOTOLINE:
				CurrLine = GotoLine(CurrLine, cCommand);
				break;
			case UPPAGE:
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = ScrollUp(CurrLine);
				break;
			case DNPAGE:
				for (i = 1; i <= cCommand && !BadMove; i++)
					CurrLine = ScrollDn(CurrLine);
				break;
			case ESC:
				cCommand = 0;
				wclear(BotWin);
				wprintw(BotWin, "Command count: 1");
				wrefresh(BotWin);
				ClearBotWin = TRUE;
				break;
			case SEARCHCHAR:
				CurrLine = Search(FirstLine,CurrLine,LastLine);
				break;
                        case REVERSEDIRANDSRCH:
				forward = !forward;
			case REPEATSEARCH:
				CurrLine = ActuallySearch(FirstLine, CurrLine,
					 LastLine);
				break;
			case HELP:
				ShowHelpScreen();
				break;
			default:
				AnnounceError("Illegal command. ");
				wprintw(BotWin, "%s", unctrl(command));
				wrefresh(BotWin);
				break;
		}
		cCommand = BadMove = 0;
		NumLines = MAX(NumLines, 1);
		LineNum = MAX(LineNum, 1);
                wrefresh(TopWin);
	}
}
			
