

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
 * SaveFile(FileName) -- rewrite the file.
 */

#include "common.h"

SaveFile(FileName)

char *FileName;

{
   int i;
   extern int Silent;
   extern int ClearBotWin;

   wclear(BotWin);
   wprintw(BotWin, "Saving file");
   if (! Silent) 
      wprintw (BotWin, " %s.  ", FileName);
   else 
      wprintw(BotWin, ".  ");

   wrefresh(BotWin);

   if (GetYorN("Type 'y' to verify save: ")) {
      Saved = TRUE;

      i = ActuallySaveFile(FileName);

      if (i) {
         char *errsb = Calloc(100);

         i = ActuallySaveFile(sbTFile);
         sprintf(errsb, "Error saving file.  Please ask for help.\nWrite down '%s.'",
            sbTFile);
         panic(errsb);
      }
      else {
         
         wclear(BotWin);
         wprintw(BotWin, "File ");
         if (! Silent) 
            wprintw (BotWin, "%s ", FileName);
         wprintw(BotWin, "saved.");
         wrefresh(BotWin);
      }
   }
   else {
      wclear(BotWin);
      wprintw(BotWin, "File not saved.");
      wrefresh(BotWin);
      }

   ClearBotWin = TRUE;

}
