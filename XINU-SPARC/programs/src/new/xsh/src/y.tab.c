extern char *malloc(), *realloc();

# line 2 "parser.y"
/* 
 * parser.y - parser for nut shell
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 *
 */

#include <stdio.h>
#include "main.h"
#include "parse.h"
#include "utils.h"
#include "hash.h"
	
extern struct	cmd_rec *cmd_node;

static struct cmd_rec *currCmd, *AddToCmdList();
static struct arglist *currArg, *AddToArgList();



# line 27 "parser.y"
typedef union  {
	char 	*s;
} YYSTYPE;
# define SWORD 257
# define DWORD 258
# define WORD 259
# define REDIRIN 260
# define REDIROUT 261
# define BACKGROUND 262
# define NEWLINE 263
# define SEMI 264
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 140 "parser.y"


    
/*
 * yyerror - needed by the parser
 */
yyerror(s)
char	*s;
{
	FreeCmdList(cmd_node);
	cmd_node = NULL;
	ReportError(s, NULL);
}


/*
 * ====================================================================
 * AddToArgList - add a new node to the argument list for this command
 * ====================================================================
 */
static struct arglist *AddToArgList(arg, type)
char	*arg;
int	type;
{
	struct arglist	*p;

	if ( currCmd->args == NULL ) {
		p = currCmd->args = (struct arglist *)
		    xmalloc(sizeof(struct arglist));
		p->arg = arg;
		p->type = type;
		p->next = NULL;
		return(p);
	}
	p = currArg->next = (struct arglist *) xmalloc(sizeof(struct arglist));
	p->arg = arg;
	p->type = type;
	p->next = NULL;
	return(p);
}


/*
 * ====================================================================
 * AddToCmdList - add a new node to the command list
 * ====================================================================
 */
static struct cmd_rec *AddToCmdList(arg, type)
char	*arg;
int 	type;
{
	struct cmd_rec *p;

	if ( cmd_node == NULL ) {
		p = cmd_node = (struct cmd_rec *)
		    xmalloc(sizeof(struct cmd_rec));
		nodeinit(cmd_node);
		cmd_node->cmd.value = arg;
		cmd_node->cmd.type = type;
		return(cmd_node);
	}
	p = currCmd->next = (struct cmd_rec *) xmalloc(sizeof(struct cmd_rec));
	nodeinit(p);
	p->cmd.value = arg;
	p->cmd.type = type;
	return(p);
}


/*
 * ====================================================================
 * nodeinit -  initialize the command node 
 * ====================================================================
 */
static nodeinit(cmd)
    struct cmd_rec *cmd;
{
	cmd->cmd.value = NULL;
	cmd->args = NULL;
	cmd->input.value = NULL;
	cmd->output.value = NULL;
	cmd->backgrounding = FALSE;
	cmd->next = NULL;
}

int yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 25
# define YYLAST 36
int yyact[]={

     4,     9,     8,     7,    10,    11,    12,     3,    20,    19,
    18,    17,    27,    30,    29,    28,    25,    24,    23,    22,
     9,     8,     7,     5,    16,    26,    21,    15,    13,     6,
     2,     1,     0,     0,     0,    14 };
int yypact[]={

  -256, -1000,  -259, -1000,  -257, -1000, -1000, -1000, -1000, -1000,
 -1000,  -237, -1000,  -249, -1000,  -242, -1000,  -241, -1000, -1000,
 -1000,  -250,  -244, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000 };
int yypgo[]={

     0,    31,    30,    23,    29,    28,    27,    26,    25,    24 };
int yyr1[]={

     0,     1,     1,     1,     2,     2,     3,     4,     4,     4,
     5,     5,     9,     9,     9,     6,     6,     6,     6,     7,
     7,     7,     7,     8,     8 };
int yyr2[]={

     0,     4,     2,     5,     6,     2,    10,     3,     3,     3,
     4,     0,     3,     3,     3,     5,     5,     5,     0,     5,
     5,     5,     0,     3,     0 };
int yychk[]={

 -1000,    -1,    -2,   263,   256,    -3,    -4,   259,   258,   257,
   263,   264,   263,    -5,    -3,    -6,    -9,   260,   259,   258,
   257,    -7,   261,   259,   258,   257,    -8,   262,   259,   258,
   257 };
int yydef[]={

     0,    -2,     0,     2,     0,     5,    11,     7,     8,     9,
     1,     0,     3,    18,     4,    22,    10,     0,    12,    13,
    14,    24,     0,    15,    16,    17,     6,    23,    19,    20,
    21 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"SWORD",	257,
	"DWORD",	258,
	"WORD",	259,
	"REDIRIN",	260,
	"REDIROUT",	261,
	"BACKGROUND",	262,
	"NEWLINE",	263,
	"SEMI",	264,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"inputline : commandlist NEWLINE",
	"inputline : NEWLINE",
	"inputline : error NEWLINE",
	"commandlist : commandlist SEMI command",
	"commandlist : command",
	"command : cmd arglist redirin redirout background",
	"cmd : WORD",
	"cmd : DWORD",
	"cmd : SWORD",
	"arglist : arglist arg",
	"arglist : /* empty */",
	"arg : WORD",
	"arg : DWORD",
	"arg : SWORD",
	"redirin : REDIRIN WORD",
	"redirin : REDIRIN DWORD",
	"redirin : REDIRIN SWORD",
	"redirin : /* empty */",
	"redirout : REDIROUT WORD",
	"redirout : REDIROUT DWORD",
	"redirout : REDIROUT SWORD",
	"redirout : /* empty */",
	"background : BACKGROUND",
	"background : /* empty */",
};
#endif /* YYDEBUG */
#line 1 "/usr/lib/yaccpar"
/*	@(#)yaccpar 1.10 89/04/04 SMI; from S5R3 1.10	*/

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	{ free(yys); free(yyv); return(0); }
#define YYABORT		{ free(yys); free(yyv); return(1); }
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** static variables used by the parser
*/
static YYSTYPE *yyv;			/* value stack */
static int *yys;			/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;			/* top of state stack */

static int yystate;			/* current state */
static int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */

int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */
	unsigned yymaxdepth = YYMAXDEPTH;

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yyv = (YYSTYPE*)malloc(yymaxdepth*sizeof(YYSTYPE));
	yys = (int*)malloc(yymaxdepth*sizeof(int));
	if (!yyv || !yys)
	{
		yyerror( "out of memory" );
		return(1);
	}
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			(void)printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			yymaxdepth += YYMAXDEPTH;
			yyv = (YYSTYPE*)realloc((char*)yyv,
				yymaxdepth * sizeof(YYSTYPE));
			yys = (int*)realloc((char*)yys,
				yymaxdepth * sizeof(int));
			if (!yyv || !yys)
			{
				yyerror( "yacc stack overflow" );
				return(1);
			}
			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			(void)printf( "Received token " );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				(void)printf( "Received token " );
				if ( yychar == 0 )
					(void)printf( "end-of-file\n" );
				else if ( yychar < 0 )
					(void)printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					(void)printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						(void)printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					(void)printf( "Error recovery discards " );
					if ( yychar == 0 )
						(void)printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						(void)printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						(void)printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			(void)printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 3:
# line 39 "parser.y"
{
					yyerrok;
				} break;
case 7:
# line 53 "parser.y"
{
					currCmd = AddToCmdList(yypvt[-0].s, WORD);
				} break;
case 8:
# line 58 "parser.y"
{
					currCmd = AddToCmdList(yypvt[-0].s, DWORD);
				} break;
case 9:
# line 63 "parser.y"
{
					currCmd = AddToCmdList(yypvt[-0].s, SWORD);
				} break;
case 12:
# line 74 "parser.y"
{
					currArg = AddToArgList(yypvt[-0].s, WORD);
				} break;
case 13:
# line 79 "parser.y"
{
					currArg = AddToArgList(yypvt[-0].s, DWORD);
				} break;
case 14:
# line 84 "parser.y"
{
					currArg = AddToArgList(yypvt[-0].s,SWORD);
				} break;
case 15:
# line 91 "parser.y"
{
					currCmd->input.value = yypvt[-0].s;
					currCmd->input.type = WORD;
				} break;
case 16:
# line 97 "parser.y"
{
					currCmd->input.value = yypvt[-0].s;
					currCmd->input.type = DWORD;
				} break;
case 17:
# line 103 "parser.y"
{
					currCmd->input.value = yypvt[-0].s;
					currCmd->input.type = SWORD;
				} break;
case 19:
# line 112 "parser.y"
{
					currCmd->output.value = yypvt[-0].s;
					currCmd->output.type = WORD;
				} break;
case 20:
# line 118 "parser.y"
{
					currCmd->output.value = yypvt[-0].s;
					currCmd->output.type = DWORD;
				} break;
case 21:
# line 124 "parser.y"
{
					currCmd->output.value = yypvt[-0].s;
					currCmd->output.type = SWORD;
				} break;
case 23:
# line 133 "parser.y"
{
					currCmd->backgrounding = TRUE;
				} break;
	}
	goto yystack;		/* reset registers in driver code */
}
