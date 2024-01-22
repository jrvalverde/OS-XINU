%{
/* 
 * scanner.l - lex specification code for nut shell
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 */


/*#define DEBUG*/

#include <stdio.h>

#include "main.h"
#include "utils.h"
#include "hash.h"
#include "tn.h"

#undef input()
#undef unput()

extern char *FixStr();

%}
%%
'(\\[^\n]|[^'\n\\])*   {		/* SINGLE QUOTES */
				if ( '\'' != input() ) {    
					ReportError("missing single quote",
						    NULL);
				        return(NEWLINE);
        	                }
                	        yylval.s = FixStr(yytext+1);
	                        return(SWORD);
        		}


\"(\\[^\n]|[^\"\n\\])* 	{		/* DOUBLE QUOTES */
	               		if ('\"' != input() ) {
					ReportError("missing double quote",
						    NULL);
				        return(NEWLINE);
                        	}
				yylval.s = FixStr(yytext+1);
				return(DWORD);
			}


(\\[^\n]|[^<>&;\"\'\n\t ])+ {		/* NO QUOTES */
				yylval.s = FixStr(yytext);
				return(WORD);
			}

[ \t]*		        ;
"<"		        return REDIRIN;
">"		        return REDIROUT;
"&"		        return BACKGROUND;
";"                     return SEMI;
\n		        return NEWLINE;
\\\n			return NEWLINE;
%%

yywrap() 
{
	return(1);
}

char *FixStr(str)
     char *str;
{
	char *p, *s;
	
	p = s = str = strsave(str);

	while (*s != '\0') {
		if ( *s != '\\' )
		    *p++ = *s++;
		else {
			s++;
			if ( *s == '\0' )
			    break;
			else if ( *s == '\n' )
			    s++;
			else 
			    *p++ = *s++;
		}
	}
	*p = '\0';
        return(str);
}

	


