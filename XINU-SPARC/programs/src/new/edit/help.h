#define cHELPLINES 17
char *mpihl[cHELPLINES] = {
"      Char  Command                    Char  Command",
"      ",
"      h*    move one character left    l*    move one character right",
"      b*    move one word left         w*    move one word right",
"      k*    move one line up           j*    move one line down",
"     ^U*    go up one half page       ^F*    go down one half page",
"      0     go to beginning of line    $     go to end of line",
"      B     go to beginning of file    E     go to end of file",
"      x*    delete a character         d*    delete the current line",
"      s     save  the file to disk     q     quit the editor",
"      i     enter insert mode          I     input a file",
"      ESC   exit insert mode           /     search for a string",
"      S     status                     g     go to a specified line",
"      ?     show this help             c     clear and redraw the screen",
"      y*    yank some lines            p*    put yanked lines",
"      n     repeat last search         N     reverse direction & repeat search",
"      o     undo last o, x, X, i, I or p"      
        };
