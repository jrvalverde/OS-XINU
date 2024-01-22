/*
 *  chars.h -- contains the key bindings for the editor.
 */

#define CHARLEFT        'h'             /* char for moving left */ 
#define CHARRIGHT       'l'             /* char for moving right */
#define CHARUP          'k'             /* char for moving up */
#define CHARDOWN        'j'             /* char for moving down */
#define WORDLEFT        'b'             /* char for word left */
#define WORDRIGHT       'w'             /* char for word right */
#define UPPAGE          '\025'             /* scroll up */
#define DNPAGE          '\006'             /* scrool down */

#define TOPOFFILE       'B'             /* go to top of file */
#define ENDOFFILE       'E'             /* go to end of file */
#define ENDOFLINE       '$'             /* char for moving to EOL */
#define BEGOFLINE       '0'             /* char for moving to BOL */
#define GOTOLINE        'g'             /* char for moving to #'d line */

#define DELETECHAR      'x'             /* delete char under cursor */
#define DELETELINE      'd'             /* delete the whole line */
#define REDRAW          'c'             /* refresh the screen (just in case!) */
#define STATUS          'S'             /* print a status message */
#define UNDODEL		'u'		/* oops!  undo the last destructive command */

#define HELP            '?'             /* help character */
#define YANK            'y'             /* yank a block of lines */
#define PUT             'p'             /* put yanked lines */
#define WRITE           's'             /* write out the workspace to file */
#define QUIT            'q'             /* get out of the editor */
#define INSERTCHAR      'i'             /* char to enter insert mode */
#define INPUTFILE	'I'		/* char to input a file */
#define ESC             '\033'          /* escape character */
#define SEARCHCHAR      '/'             /* char for search */
#define REPEATSEARCH	'n'		/* repeat last search */
#define REVERSEDIRANDSRCH 'N'		/* reverse dir then n */
