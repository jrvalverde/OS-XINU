

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

#include <tty.h>

extern int oldstdinbufsiz;

/*
 * Quit() -- get out of the editor.
 */

#include "common.h"

Quit()

{
   extern errno;

   xcontrol(0, TCMODEC);
   xcontrol(0, TCECHO);
   stdin->_bufsiz = oldstdinbufsiz;
   if (Saved) {
      clear();
      refresh();
      endwin();
      if (unlink(sbTFile) && errno != ENOENT) 
         perror(sbTFile);
      signal(SIGINT, SIG_DFL);
      signal(SIGTSTP, SIG_DFL);
      exit(0);
      }
   else {
      wclear(BotWin);
      if (GetYorN("File has not been not saved. Do you really want to quit? ")) {
         clear();
         refresh();
         endwin();
         if (unlink(sbTFile) && errno != ENOENT) 
            perror(sbTFile);
         signal(SIGINT, SIG_DFL);
         signal(SIGTSTP, SIG_DFL);
         exit(0);
         }
      else {
         wclear(BotWin);
         wrefresh(BotWin);
         }
   }
}
