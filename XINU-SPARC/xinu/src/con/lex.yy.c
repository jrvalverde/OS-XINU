# include "stdio.h"
# define U(x) ((x)&0377)
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin ={stdin}, *yyout ={stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
	int skipping;
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
		{skipping = 1;}
break;
case 2:
		{skipping = 0;}
break;
case 3:
		{extern	int	brkcount;
			 if (! skipping)
				if (++brkcount == 2)
					return(0); /* end-of-file */
				else
					return(DEFBRK);
			}
break;
case 4:
		{if (! skipping) return(COLON);}
break;
case 5:
		{if (! skipping) return(LBRACE);}
break;
case 6:
		{if (! skipping) return(RBRACE);}
break;
case 7:
	{if (! skipping) return(HEX);}
break;
case 8:
		{if (! skipping) return(OCTAL);}
break;
case 9:
	{if (! skipping) return(INTEGER);}
break;
case 10:
		{if (! skipping) return(IS);}
break;
case 11:
		{if (! skipping) return(ON);}
break;
case 12:
		;
break;
case 13:
		{if (! skipping) return(IINT);}
break;
case 14:
		{if (! skipping) return(IINT);}
break;
case 15:
		{if (! skipping) return(OINT);}
break;
case 16:
		{if (! skipping) return(OINT);}
break;
case 17:
		{if (! skipping) return(CSR);}
break;
case 18:
		{if (! skipping) return(CSR);}
break;
case 19:
		{if (! skipping) return(IVEC);}
break;
case 20:
		{if (! skipping) return(IVEC);}
break;
case 21:
		{if (! skipping) return(OVEC);}
break;
case 22:
		{if (! skipping) return(OVEC);}
break;
case 23:
		{if (! skipping) return(INIT);}
break;
case 24:
		{if (! skipping) return(INIT);}
break;
case 25:
		{if (! skipping) return(OPEN);}
break;
case 26:
		{if (! skipping) return(OPEN);}
break;
case 27:
		{if (! skipping) return(CLOSE);}
break;
case 28:
		{if (! skipping) return(CLOSE);}
break;
case 29:
		{if (! skipping) return(READ);}
break;
case 30:
		{if (! skipping) return(READ);}
break;
case 31:
		{if (! skipping) return(GETC);}
break;
case 32:
		{if (! skipping) return(GETC);}
break;
case 33:
		{if (! skipping) return(PUTC);}
break;
case 34:
		{if (! skipping) return(PUTC);}
break;
case 35:
		{if (! skipping) return(WRITE);}
break;
case 36:
		{if (! skipping) return(WRITE);}
break;
case 37:
		{if (! skipping) return(SEEK);}
break;
case 38:
		{if (! skipping) return(SEEK);}
break;
case 39:
		{if (! skipping) return(CNTL);}
break;
case 40:
		{if (! skipping) return(CNTL);}
break;
case 41:
		;
break;
case 42:
		{extern int linectr;
			 linectr++;
			}
break;
case 43:
		{if (! skipping) return(IDENT);}
break;
case 44:
		{if (! skipping) return(yytext[0]);}
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */
int yyvstop[] ={
0,

44,
0,

41,
44,
0,

42,
0,

3,
44,
0,

44,
0,

44,
0,

44,
0,

8,
44,
0,

9,
44,
0,

4,
44,
0,

12,
44,
0,

43,
44,
0,

5,
44,
0,

6,
44,
0,

28,
43,
44,
0,

32,
43,
44,
0,

24,
43,
44,
0,

40,
43,
44,
0,

26,
43,
44,
0,

34,
43,
44,
0,

30,
43,
44,
0,

38,
43,
44,
0,

36,
43,
44,
0,

41,
0,

2,
0,

27,
0,

31,
0,

23,
0,

39,
0,

25,
0,

33,
0,

29,
0,

37,
0,

35,
0,

1,
0,

8,
0,

7,
0,

9,
0,

43,
0,

43,
0,

43,
0,

10,
43,
0,

43,
0,

43,
0,

11,
43,
0,

43,
0,

18,
43,
0,

43,
0,

43,
0,

43,
0,

43,
0,

17,
0,

13,
43,
0,

20,
43,
0,

15,
43,
0,

22,
43,
0,

14,
0,

19,
0,

16,
0,

21,
0,
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] ={
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,26,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	4,26,	
0,0,	0,0,	1,6,	0,0,	
0,0,	0,0,	0,0,	1,7,	
9,37,	0,0,	1,8,	0,0,	
1,9,	1,10,	1,11,	7,27,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,12,	
0,0,	0,0,	1,13,	2,6,	
0,0,	0,0,	1,14,	0,0,	
2,7,	0,0,	0,0,	2,8,	
1,14,	2,9,	11,40,	11,40,	
11,40,	11,40,	11,40,	11,40,	
11,40,	11,40,	11,40,	11,40,	
2,12,	0,0,	0,0,	2,13,	
0,0,	0,0,	0,0,	0,0,	
1,15,	0,0,	1,16,	0,0,	
0,0,	0,0,	0,0,	0,0,	
1,17,	0,0,	0,0,	0,0,	
1,18,	0,0,	1,19,	0,0,	
0,0,	0,0,	0,0,	1,20,	
1,21,	1,22,	19,43,	1,23,	
1,24,	2,15,	45,56,	2,16,	
1,25,	21,46,	17,42,	28,49,	
19,44,	2,17,	21,47,	19,45,	
43,55,	2,18,	30,50,	2,19,	
42,54,	46,57,	21,48,	48,58,	
2,20,	2,21,	2,22,	32,52,	
2,23,	2,24,	8,28,	30,51,	
49,59,	2,25,	8,29,	50,60,	
8,30,	51,61,	52,62,	53,63,	
32,53,	8,31,	8,32,	8,33,	
55,64,	8,34,	8,35,	56,65,	
57,66,	58,67,	8,36,	10,38,	
10,38,	10,38,	10,38,	10,38,	
10,38,	10,38,	10,38,	10,38,	
10,38,	38,38,	38,38,	38,38,	
38,38,	38,38,	38,38,	38,38,	
38,38,	38,38,	38,38,	60,68,	
61,69,	62,70,	63,71,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	0,0,	0,0,	
0,0,	0,0,	14,41,	10,39,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	39,39,	39,39,	
39,39,	39,39,	39,39,	39,39,	
39,39,	39,39,	39,39,	39,39,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	39,39,	
39,39,	39,39,	39,39,	39,39,	
39,39,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	39,39,	
39,39,	39,39,	39,39,	39,39,	
39,39,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] ={
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-26,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+4,	0,		yyvstop+11,
yycrank+43,	0,		yyvstop+13,
yycrank+2,	0,		yyvstop+15,
yycrank+115,	0,		yyvstop+17,
yycrank+26,	0,		yyvstop+20,
yycrank+0,	0,		yyvstop+23,
yycrank+0,	0,		yyvstop+26,
yycrank+139,	0,		yyvstop+29,
yycrank+0,	0,		yyvstop+32,
yycrank+0,	0,		yyvstop+35,
yycrank+7,	yysvec+14,	yyvstop+38,
yycrank+0,	yysvec+14,	yyvstop+42,
yycrank+9,	yysvec+14,	yyvstop+46,
yycrank+0,	yysvec+14,	yyvstop+50,
yycrank+16,	yysvec+14,	yyvstop+54,
yycrank+0,	yysvec+14,	yyvstop+58,
yycrank+0,	yysvec+14,	yyvstop+62,
yycrank+0,	yysvec+14,	yyvstop+66,
yycrank+0,	yysvec+14,	yyvstop+70,
yycrank+0,	yysvec+4,	yyvstop+74,
yycrank+0,	0,		yyvstop+76,
yycrank+8,	0,		yyvstop+78,
yycrank+0,	0,		yyvstop+80,
yycrank+25,	0,		yyvstop+82,
yycrank+0,	0,		yyvstop+84,
yycrank+34,	0,		yyvstop+86,
yycrank+0,	0,		yyvstop+88,
yycrank+0,	0,		yyvstop+90,
yycrank+0,	0,		yyvstop+92,
yycrank+0,	0,		yyvstop+94,
yycrank+0,	0,		yyvstop+96,
yycrank+125,	0,		yyvstop+98,
yycrank+214,	0,		yyvstop+100,
yycrank+0,	yysvec+11,	yyvstop+102,
yycrank+0,	yysvec+14,	yyvstop+104,
yycrank+18,	yysvec+14,	yyvstop+106,
yycrank+18,	yysvec+14,	yyvstop+108,
yycrank+0,	yysvec+14,	yyvstop+110,
yycrank+17,	yysvec+14,	yyvstop+113,
yycrank+23,	yysvec+14,	yyvstop+115,
yycrank+0,	yysvec+14,	yyvstop+117,
yycrank+34,	yysvec+14,	yyvstop+120,
yycrank+30,	0,		0,	
yycrank+37,	0,		0,	
yycrank+48,	0,		0,	
yycrank+40,	0,		0,	
yycrank+50,	0,		0,	
yycrank+0,	yysvec+14,	yyvstop+122,
yycrank+40,	yysvec+14,	yyvstop+125,
yycrank+60,	yysvec+14,	yyvstop+127,
yycrank+44,	yysvec+14,	yyvstop+129,
yycrank+62,	yysvec+14,	yyvstop+131,
yycrank+0,	0,		yyvstop+133,
yycrank+67,	0,		0,	
yycrank+85,	0,		0,	
yycrank+69,	0,		0,	
yycrank+87,	0,		0,	
yycrank+0,	yysvec+14,	yyvstop+135,
yycrank+0,	yysvec+14,	yyvstop+138,
yycrank+0,	yysvec+14,	yyvstop+141,
yycrank+0,	yysvec+14,	yyvstop+144,
yycrank+0,	0,		yyvstop+147,
yycrank+0,	0,		yyvstop+149,
yycrank+0,	0,		yyvstop+151,
yycrank+0,	0,		yyvstop+153,
0,	0,	0};
struct yywork *yytop = yycrank+316;
struct yysvf *yybgin = yysvec+1;
char yymatch[] ={
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
'0' ,'1' ,'1' ,'1' ,'1' ,'1' ,'1' ,'1' ,
'1' ,'1' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,01  ,01  ,01  ,01  ,'G' ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,'G' ,
'G' ,'G' ,'G' ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] ={
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	ncform	4.1	83/08/11	*/

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank){		/* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"unsigned char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
