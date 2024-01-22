/*
 *  This is the header file for the programming contest editor.
 *
 *  24 Jun 86
 *  Steve Chapin  (sjc@purdue.EDU)
 */

#define MAXLINE         79              /* max length of a line */
#define TOPWINLINES     (LINES - 5)     /* no. of lines in top window */
#define BOTWINLINES     3
#define NULLCHAR        0               /* character for end of string */

int CursorX,                            /* X position of cursor */
    CursorY,                            /* Y postion of cursor */
    LineNum,                            /* line number */
    NumLines,                           /* total number of lines in the file */
    cKeystrokes,                        /* counter for writing of tmp file */
    LinePos;                            /* Index into line */

struct line {
        char            text[MAXLINE];  /* text of the line */
        int             length;         /* length of the line */
        struct line     *next;          /* pointer to next line */
        struct line     *prev;          /* pointer to prev. line */
        };

struct line     *FirstLine,             /* first line in file */
                *LastLine,              /* last line in file */
                *CurrLine,              /* current line */
                *TopLineOnScreen,       /* line at top of screen */
                *BotLineOnScreen,       /* last line on screen */
                *Buffer;                /* yanking buffer */

int Saved,                              /* true if the file is unchanged
                                           since it was last saved */
    Silent,				/* true if in silent mode */
    BadMove,				/* true if a move in a direction was
					   illegal.  used to exit loops */
    ClearBotWin,			/* flag to tell if bottom window
					   should be cleared before next
					   command execution */
    forward,				/* forward flag for search */
    cCommand;				/* count of commands for repetition */

typedef struct {			/* extra stuff for the undo info */
	int	tag;		/* tag indicating either o, x, X, i, I, p */
	union {
		struct {	/* delete char */
			struct line *first,	/* the first line changed */
			*del,		   	/* the deleted text */
			*last;		   	/* last line that was changed */
			int bLp, 	   	/* beginning line position */
 			eLp;  		   	/* ending line position */

		} u_delchar;
		struct {	/* insert chars. */
			struct line *first,	/* first line changed */
                        *del,			/* deleted text */
			*last;			/* last line changed */
			int bLp, 	   	/* beginning line position */
 			eLp;  		   	/* ending line position */
		} u_inschar;
		struct {	/* delete line */
			struct line *prev; /* the previous line */
			struct line *first; /* current line */
			struct line *last; /* next line */
		} u_delline;
				/* put and Input file can both be
				   handled as cases of Undo Delete Line */
	} u;
} Extra;

Extra *UndoInfo;
	
char *Calloc(),
     *malloc(),
     *VBsb,				/* visual bell string */
     *FileName,                         /* name of file */
     *sbLFile,				/* log file */
     *srchstr,				/* search string */
     *sbTFile;                          /* tmp file for backup */

#define READFILE "r"
#define WRITEFILE "w"
#define EMPTYSTRING ""
FILE *fopen();
FILE *InputFile;
FILE *Logfile;

WINDOW  *TopWin,                        /* Top window, where editing is done */
        *MidWin,                        /* just the dividing line */
	*StatWin,			/* displays the current mode */
        *BotWin,                        /* status message, etc. */
        *HelpWin;                       /* help screen */

#define MIN(a,b) (a < b) ? a : b
#define MAX(a,b) (a > b) ? a : b
#define beep() printf("%s", VBsb)

#define tagDELETELINE 1
#define tagDELETECHAR 2
#define tagPUT 3
#define tagINPUT 4
#define tagINSERT 5
#define tagUNDODELETELINE 11
#define tagUNDODELETECHAR 12
#define tagUNDOPUT 13
#define tagUNDOINPUT 14
#define tagUNDOINSERT 15
