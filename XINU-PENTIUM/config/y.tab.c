#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93 (BSDI)";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYEMPTY (-1)
#define YYLEX yylex()
#define yyclearin (yychar=YYEMPTY)
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 4 "config.y"
#include <stdio.h>

#define	NIL	(dvptr)0

#define	CONFIGC		"conf.c"		/* name of .c output	*/
#define	CONFIGH		"../h/conf.h"		/* name of .h output	*/
#define	CONFHREF	"<conf.h>"		/* how conf.h referenced*/
#define	CONFIGIN	"Configuration"		/* name of input file	*/

#define	IRQBASE		32

FILE	*confc;
FILE	*confh;

char	*dbstr;
int	ndevs = 0;
int	currname = -1;
int	currtname = -1;
int	currdname = -1;
int	brkcount = 0;

struct	syment	{			/* symbol table			*/
	char	*symname;
	int	symoccurs;
	} symtab[250];

int	nsym = 0;
int	lookup();
int	linectr = 1;
char	*doing = "device type declaration";
char	*s;
struct	dvtype {
	char		*dvname;	/* device name (not used in types)*/
	char		*dvtname;	/* type name			*/
	int		dvtnum;		/* symbol table index of type	*/
	char		*dvdevice;	/* device name			*/
	int		dvcsr;		/* Control Status Register addr	*/
	int		dvivec;		/* input interrupt vector	*/
	int		dvovec;		/* Output interrupt vector	*/
	char		dviint[20];	/* input interrupt routine	*/
	char		dvoint[20];	/* output interrupt routine	*/
	char		dvinit[20];	/* init routine name		*/
	char		dvopen[20];	/* open routine name		*/
	char		dvclose[20];	/* close routine name		*/
	char		dvread[20];	/* read routine name		*/
	char		dvwrite[20];	/* write routine name		*/
	char		dvcntl[20];	/* control routine name		*/
	char		dvseek[20];	/* seek routine name		*/
	char		dvgetc[20];	/* getc routine name		*/
	char		dvputc[20];	/* putc routine name		*/
	int		dvminor;	/* device number 0,1,...	*/
	struct dvtype	*dvnext;	/* next node on the list	*/
	};
typedef	struct	dvtype	*dvptr;
	dvptr	ftypes = NIL;		/* linked list of device types	*/
	dvptr	devs = NIL;		/* linked list of device decls.	*/
	dvptr	lastdv = NIL;
	dvptr	currtype = NIL;

char	*ftout[] = 
	       {"struct\tdevsw\t{\t\t\t/* device table entry */\n",
	        "\tint\tdvnum;\n",
		"\tchar\t*dvname;\n",
		"\tint\t(*dvinit)();\n",
		"\tint\t(*dvopen)();\n",
		"\tint\t(*dvclose)();\n",
		"\tint\t(*dvread)();\n",
		"\tint\t(*dvwrite)();\n",
		"\tint\t(*dvseek)();\n",
		"\tint\t(*dvgetc)();\n",
		"\tint\t(*dvputc)();\n",
		"\tint\t(*dvcntl)();\n",
		"\tint\tdvcsr;\n",
		"\tint\tdvivec;\n",
		"\tint\tdvovec;\n",
		"\tint\t(*dviint)();\n",
		"\tint\t(*dvoint)();\n",
		"\tchar\t*dvioblk;\n",
		"\tint\tdvminor;\n",
		"\t};\n\n",
		"extern\tstruct\tdevsw devtab[];",
		"\t\t/* one entry per device */\n\n",
		NULL};

int atoi();

#line 101 "y.tab.c"
#define DEFBRK 257
#define COLON 258
#define OCTAL 259
#define INTEGER 260
#define IDENT 261
#define CSR 262
#define IVEC 263
#define OVEC 264
#define IRQ 265
#define IINT 266
#define OINT 267
#define INIT 268
#define OPEN 269
#define CLOSE 270
#define READ 271
#define WRITE 272
#define SEEK 273
#define CNTL 274
#define IS 275
#define ON 276
#define GETC 277
#define PUTC 278
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    1,    3,    3,    4,    6,    6,    7,    5,    9,
    8,    8,   10,   10,   10,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10,   10,   11,    2,    2,
   12,   13,   14,   15,   15,
};
short yylen[] = {                                         2,
    2,    2,    0,    2,    2,    2,    3,    2,    2,    1,
    0,    2,    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    1,    0,    2,
    2,    4,    1,    0,    2,
};
short yydefred[] = {                                      3,
    0,   29,    0,    0,    2,   10,    4,    0,    0,   33,
   30,   11,    0,    0,    0,   11,    9,    0,    0,    8,
   11,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   12,    0,    0,
   28,   13,   14,   15,   16,   17,   18,   21,   19,   20,
   24,   25,   26,   27,   22,   23,    0,   32,   35,
};
short yydgoto[] = {                                       1,
    2,    4,    3,    7,    8,   15,   16,   18,    9,   38,
   42,   11,   12,   13,   58,
};
short yysindex[] = {                                      0,
    0,    0, -248, -255,    0,    0,    0, -268, -247,    0,
    0,    0, -259, -255, -268,    0,    0, -226, -255,    0,
    0, -226, -240, -240, -240, -240, -255, -255, -255, -255,
 -255, -255, -255, -255, -255, -255, -255,    0, -253, -226,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -255,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,   21,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -243,    0,    0,   15,    0,    0,
    0, -257,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    1, -254,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    9,  -11,   -2,    0,
   32,    0,    0,    0,    0,
};
#define YYTABLESIZE 279
short yytable[] = {                                       6,
   34,   10,    7,    6,   22,    6,    7,   14,    5,   40,
   17,   20,    6,    5,   31,   19,   39,    5,    6,   41,
    1,    7,   57,   21,   46,   47,   48,   49,   50,   51,
   52,   53,   54,   55,   56,   23,   24,   25,   26,   27,
   28,   29,   30,   31,   32,   33,   34,   35,    0,    0,
   36,   37,    0,    0,   59,   43,   44,   45,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   34,   34,   34,   34,   34,   34,   34,   34,   34,
   34,   34,   34,   34,   34,   31,    0,   34,   34,
};
short yycheck[] = {                                     257,
    0,    4,  257,  261,   16,  261,  261,  276,  257,   21,
  258,   14,  261,  257,    0,  275,   19,  261,  276,  260,
    0,  276,  276,   15,   27,   28,   29,   30,   31,   32,
   33,   34,   35,   36,   37,  262,  263,  264,  265,  266,
  267,  268,  269,  270,  271,  272,  273,  274,   -1,   -1,
  277,  278,   -1,   -1,   57,   24,   25,   26,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  261,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  261,   -1,  277,  278,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 278
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"DEFBRK","COLON","OCTAL",
"INTEGER","IDENT","CSR","IVEC","OVEC","IRQ","IINT","OINT","INIT","OPEN","CLOSE",
"READ","WRITE","SEEK","CNTL","IS","ON","GETC","PUTC",
};
char *yyrule[] = {
"$accept : config.input",
"config.input : devicetypes devicedescriptors",
"devicetypes : ftypes DEFBRK",
"ftypes :",
"ftypes : ftypes ftype",
"ftype : tname device.list",
"device.list : devheader attribute.list",
"device.list : device.list devheader attribute.list",
"devheader : ON id",
"tname : id COLON",
"id : IDENT",
"attribute.list :",
"attribute.list : attribute.list attribute",
"attribute : CSR number",
"attribute : IVEC number",
"attribute : OVEC number",
"attribute : IRQ number",
"attribute : IINT id",
"attribute : OINT id",
"attribute : OPEN id",
"attribute : CLOSE id",
"attribute : INIT id",
"attribute : GETC id",
"attribute : PUTC id",
"attribute : READ id",
"attribute : WRITE id",
"attribute : SEEK id",
"attribute : CNTL id",
"number : INTEGER",
"devicedescriptors :",
"devicedescriptors : devicedescriptors descriptor",
"descriptor : fspec attribute.list",
"fspec : dname IS id optional.on",
"dname : id",
"optional.on :",
"optional.on : ON id",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 170 "config.y"
#include "lex.yy.c"
main(argc, argv)
	int	argc;
	char	*argv[];
{
	int	n, i, j, l, fcount;
	dvptr	s;
	int	verbose = 0;
	char	*p;
	char	c;

	if (argc>1 && (strcmp("-v",argv[1])==0)) {
		argc--;
		argv++;
		verbose++;
	}
	if (argc>2) {
		fprintf(stderr,"use: config [-v] [file]\n");
		exit(1);
	}
	if (verbose)
		printf("Opening input file...\n");
	if (argc == 2) {
		if (freopen(argv[1], "r", stdin) == NULL) {
			fprintf(stderr,"Can't open %s\n",argv[1]);
			exit(1);
		}
	} else {	/* try to open Configuration file */
		if (freopen(CONFIGIN, "r", stdin) == NULL) {
			fprintf(stderr,"Can't open %s\n", CONFIGIN);
			exit(1);
		}
	}

	/* Parse the Configuration file */

	if (verbose)
		printf("Parsing configuration specs...\n");
	if ((n=yyparse()) != 0)
		exit(n);

	/* write config.h and config.c */

	if (verbose)
		printf("Opening output files...\n");
	if ( (confc=fopen(CONFIGC,"w") ) == NULL) {
		fprintf(stderr, "Can't write on %s\n", CONFIGC);
		exit(1);
	}

	if ( (confh=fopen(CONFIGH,"w") ) == NULL) {
		fprintf(stderr, "Can't write on %s\n", CONFIGH);
		exit(1);
	}
	fprintf(confh,
		"/* conf.h (GENERATED FILE; DO NOT EDIT) */\n");
	fprintf(confc,
		"/* conf.c (GENERATED FILE; DO NOT EDIT) */\n");
	fprintf(confc, "\n#include %s\n", CONFHREF);
	fprintf(confh, "\n#define\tNULLPTR\t(char *)0\n");


	if (verbose)
		printf("Writing output...\n");
	fprintf(confh,"\n/* Device table declarations */\n");
	for (i=0 ; (p=ftout[i])!=NULL ; i++)
		fprintf(confh, "%s", p);

	/* write device declarations and definitions; count type refs. */

	fprintf(confh, "\n/* Device name definitions */\n\n");
	for (i=0,s=devs; s!=NIL ; s=s->dvnext,i++) {
		fprintf(confh, "#define\t%-12s%d\t\t\t/* type %-8s */\n",
			s->dvname, i, s->dvtname);
		s->dvminor = symtab[s->dvtnum].symoccurs++;
	}

	/* write count of device types */

	fprintf(confh,"\n/* Control block sizes */\n\n");
	for (i=0 ; i<nsym ; i++)
		if (symtab[i].symoccurs > 0) {
			fprintf(confh, "#define\tN%s\t%d\n",
				symtab[i].symname, symtab[i].symoccurs);
		}
	if (ndevs > 0)
		fprintf(confh, "\n#define\tNDEVS\t%d\n\n", ndevs);

	/* empty symbol table, collect, and write names of all I/O routines */

	nsym = 0;
	for (s=devs; s!=NIL ; s=s->dvnext) {
		lookup(s->dvinit,strlen(s->dvinit));
		lookup(s->dvopen,strlen(s->dvopen));
		lookup(s->dvclose,strlen(s->dvclose));
		lookup(s->dvread,strlen(s->dvread));
		lookup(s->dvwrite,strlen(s->dvwrite));
		lookup(s->dvseek,strlen(s->dvseek));
		lookup(s->dvcntl,strlen(s->dvcntl));
		lookup(s->dvgetc,strlen(s->dvgetc));
		lookup(s->dvputc,strlen(s->dvputc));
		lookup(s->dviint,strlen(s->dviint));
		lookup(s->dvoint,strlen(s->dvoint));
		
	}
	fprintf(confh,
		"/* Declarations of I/O routines referenced */\n\n");
	for (i=0 ; i<nsym ; i++)
		fprintf(confh, "extern\tint\t%s();\n", symtab[i].symname);


	/* produce devtab (giant I/O switch table) */

	fprintf(confc, "\n/* device independent I/O switch */\n\n");
	if (ndevs > 0) {
		fprintf(confc, "struct\tdevsw\tdevtab[NDEVS] = {\n");
		fprintf(confc, "\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
			"/*  Format of entries is:",
			"device-number, device-name,",
			"init, open, close,",
			"read, write, seek,",
			"getc, putc, cntl,",
			"device-csr-address, input-vector, output-vector,",
			"iint-handler, oint-handler, control-block, minor-device,",
			"*/");
	}
	for (fcount=0,s=devs ; s!=NIL ; s=s->dvnext,fcount++) {
		fprintf(confc, "\n/*  %s  is %s  */\n\n",s->dvname,s->dvtname);
		fprintf(confc, "%d, \"%s\",\n", fcount, s->dvname);
		fprintf(confc, "%s, %s, %s,\n",
			s->dvinit, s->dvopen, s->dvclose);
		fprintf(confc, "%s, %s, %s,\n",
			s->dvread, s->dvwrite, s->dvseek);
		fprintf(confc, "%s, %s, %s,\n",
			s->dvgetc, s->dvputc, s->dvcntl);
		fprintf(confc, "0%06o, 0%03o, 0%03o,\n",
			s->dvcsr, s->dvivec, s->dvovec);
		fprintf(confc, "%s, %s, NULLPTR, %d",
			s->dviint, s->dvoint, s->dvminor);
		if ( s->dvnext != NIL )
			fprintf(confc, ",\n");
		else
			fprintf(confc, "\n\t};");
	}

	/* Copy definitions to output */

	if (brkcount == 2 && verbose)
		printf("Copying definitions to %s...\n", CONFIGH);
	if (brkcount == 2 )
		while ( (c=input()) > 0)	/* lex input routine */
			putc(c, confh);

	/* guarantee conf.c written later than conf.c for make */

	fclose(confh);
	fprintf(confc, "\n");
	fclose(confc);

	/* finish up and write report for user if requested */

	if (verbose) {
	    printf("\nConfiguration complete. Number of devs=%d:\n\n",ndevs);
	    for (s=devs; s!=NIL ; s=s->dvnext)
	      printf(
	        "Device %s (on %s) csr=0%-7o, ivec=0%-3o, ovec=0%-3o, minor=%d\n",
		s->dvname, s->dvdevice, s->dvcsr, s->dvivec, s->dvovec,
		s->dvminor);
	}
	

}


yyerror(s)
	char *s;
{
	fprintf(stderr,"Syntax error in %s on line %d\n",
		doing,linectr);
}


/*  lookup  --  lookup a name in the symbol table; return position */

lookup(str,len)
	char	*str;
	int	len;
{
	int	i;
	char	*s;

	if (len >= 20) {
		len = 19;
		fprintf(stderr,"warning: name %s truncated\n",str);
		}
	s = (char *)malloc(len+1);
	strncpy(s,str,len);
	s[len] = '\000';
	for (i=0 ; i<nsym ; i++)
		if (strcmp(s,symtab[i].symname) == 0){
			return(i);
		}
	symtab[nsym].symname = s;
	symtab[nsym].symoccurs = 0;
	return(nsym++);
}

int
atoi(p, len)
char	*p;
int	len;
{
	int	base, rv;

	if (*p == '0') {
		++p; --len;
		if (*p == 'x' || *p == 'X') {
			++p; --len;		/* skip 'x' */
			base = 16;
		} else
			base = 8;
	} else
		base = 10;
	rv = 0;
	for (; len > 0; ++p, --len) {
		rv *= base;
		if (isdigit(*p))
			rv += *p - '0';
		else if (isupper(*p))
			rv += *p - 'A' + 10;
		else
			rv += *p - 'a' + 10;
	}
	return rv;
}

/* newattr -- add a new attribute spec to current type/device description	*/

newattr(tok,val)
	int	tok;			/* token type (attribute type)	*/
	int	val;			/* symbol number of value	*/
{
	char	*c;
	dvptr	s;

	if (devs == NIL)		/* doing types			*/
		s = currtype;
	else
		s = lastdv;
	if (val>=0 && val<nsym) {
		c = symtab[val].symname;
		if (strlen(c) > 20 ) {
			fprintf(stderr,"Internal overflow\n");
			exit(1);
		}
	} else
		c = NULL;

	switch (tok) {

	case CSR:	s->dvcsr = val;
			break;
	case IVEC:	s->dvivec = val;
			break;
	case OVEC:	s->dvovec = val;
			break;
	case IRQ:	s->dvovec = s->dvivec = val;
			break;
	case IINT:	strcpy(s->dviint,c);
			break;
	case OINT:	strcpy(s->dvoint,c);
			break;
	case READ:	strcpy(s->dvread,c);
			break;
	case WRITE:	strcpy(s->dvwrite,c);
			break;
	case GETC:	strcpy(s->dvgetc,c);
			break;
	case PUTC:	strcpy(s->dvputc,c);
			break;
	case OPEN:	strcpy(s->dvopen,c);
			break;
	case CLOSE:	strcpy(s->dvclose,c);
			break;
	case INIT:	strcpy(s->dvinit,c);
			break;
	case SEEK:	strcpy(s->dvseek,c);
			break;
	case CNTL:	strcpy(s->dvcntl,c);
			break;
	default:	fprintf(stderr, "Internal error 1\n");
	}
}

/* cktname  --  check type name for duplicates */

cktname(symid)
	int symid;
{
	dvptr	s;
extern	dvptr	ftypes;
	char	*name;

	name = symtab[symid].symname;
	for (s=ftypes; s!=NIL ; s=s->dvnext) {
		if (s->dvtname == name) {
			fprintf(stderr,"Duplicate type name %s on line %d\n",
				name,linectr);
			exit(1);
			}
		}
	return(symid);
}

/* mktype  --  make a node in the type list and initialize to defaults	*/

mktype(deviceid)
	int deviceid;
{
	dvptr	s,p;
	char	*tn,*dn;

	p = NIL;
	tn = symtab[currtname].symname;
	dn = symtab[deviceid].symname;
	for (s = ftypes; s!=NIL ; s=s->dvnext) {
		if (s->dvtname == tn && s->dvdevice==dn) {
		   fprintf(stderr,
		     "Duplicate device %s for type %s on line %d\n",
		     dn, tn, linectr);
		   exit(1);
		}
		p = s;
	}
	currtype = s = (dvptr) malloc( sizeof(struct dvtype));
	if (ftypes != NIL) {
		p->dvnext = s;
	}
	else {
		ftypes = s;
	}
	initattr(s, currtname, deviceid);
}

/* initialize attributes in a type declaration node to typename...	*/

initattr(fstr, tnum, deviceid)
	dvptr	fstr;
	int	tnum;
	int	deviceid;
{
	char	*typnam;

	typnam = symtab[tnum].symname;
	fstr->dvname = NULL;
	fstr->dvtname = typnam;
	fstr->dvtnum = tnum;
	fstr->dvdevice = symtab[deviceid].symname;
	fstr->dvcsr = 0;
	fstr->dvivec = 0;
	fstr->dvovec = 0;
	strcpy(fstr->dviint,typnam);
	strcat(fstr->dviint,"iin");
	strcpy(fstr->dvoint,typnam);
	strcat(fstr->dvoint,"oin");
	strcpy(fstr->dvinit,typnam);
	strcat(fstr->dvinit,"init");
	strcpy(fstr->dvopen,typnam);
	strcat(fstr->dvopen,"open");
	strcpy(fstr->dvclose,typnam);
	strcat(fstr->dvclose,"close");
	strcpy(fstr->dvread,typnam);
	strcat(fstr->dvread,"read");
	strcpy(fstr->dvwrite,typnam);
	strcat(fstr->dvwrite,"write");
	strcpy(fstr->dvcntl,typnam);
	strcat(fstr->dvcntl,"control");
	strcpy(fstr->dvseek,typnam);
	strcat(fstr->dvseek,"seek");
	strcpy(fstr->dvgetc,typnam);
	strcat(fstr->dvgetc,"getc");
	strcpy(fstr->dvputc,typnam);
	strcat(fstr->dvputc,"putc");
	fstr->dvminor = 0;
}

/* mkdev  --  make a node on the device list */

mkdev(nameid, typid, deviceid)
	int	nameid, typid, deviceid;
{
	dvptr	s;
	char	*devn,*tn,*dn;
	int	found;

	s = (dvptr) malloc(sizeof(struct dvtype));
	s->dvnext = NIL;
	if (devs == NIL) {
		devs = s;
		lastdv = s;
	} else {
		lastdv->dvnext = s;
		lastdv = s;
	}
	ndevs++;
	tn = symtab[typid].symname;
	devn = symtab[nameid].symname;
	if (deviceid >= 0)
		dn = symtab[deviceid].symname;
	else
		dn = NULL;
	found = 0;
	for (s=ftypes ; s != NULL ; s=s->dvnext)
		if (s->dvtname == tn && (dn==NULL || s->dvdevice==dn)) {
			strdup(lastdv,s,sizeof(struct dvtype));
			found=1;
			break;
		}
	if (found==0) {
		fprintf(stderr,
		  "Bad type or device name in declaration of %s on line %d\n",
		  devn, linectr);
		exit(1);
	}
	lastdv->dvnext = NIL;
	lastdv->dvname = devn;
}


/* chdname  -- check for duplicate device name */

ckdname(devid)
	int devid;
{
	dvptr	s;
extern	dvptr	devs;
	char	*name;

	name = symtab[devid].symname;
	for (s=devs; s!=NIL ; s=s->dvnext) {
		if (s->dvname == name) {
			fprintf(stderr,"Duplicate device name %s on line %d\n",
				name,linectr);
			exit(1);
			}
		}
	return(devid);
}

strdup(tostr,fromstr,len)
	char	*tostr, *fromstr;
	int	len;
{
	for( ; len > 0 ; len--)
		*tostr++ = *fromstr++;
}
#line 768 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 2:
#line 95 "config.y"
{doing = "device definitions";}
break;
case 8:
#line 106 "config.y"
{mktype(yyvsp[0]);}
break;
case 9:
#line 109 "config.y"
{yyval = currtname = cktname(yyvsp[-1]);}
break;
case 10:
#line 112 "config.y"
{yyval = currname =
					 lookup(yytext,yyleng);
					}
break;
case 13:
#line 120 "config.y"
{newattr(CSR,yyvsp[0]);}
break;
case 14:
#line 122 "config.y"
{newattr(IVEC,yyvsp[0]);}
break;
case 15:
#line 124 "config.y"
{newattr(OVEC,yyvsp[0]);}
break;
case 16:
#line 126 "config.y"
{newattr(IRQ,yyvsp[0] +IRQBASE);}
break;
case 17:
#line 128 "config.y"
{newattr(IINT,yyvsp[0]);}
break;
case 18:
#line 130 "config.y"
{newattr(OINT,yyvsp[0]);}
break;
case 19:
#line 132 "config.y"
{newattr(OPEN,yyvsp[0]);}
break;
case 20:
#line 134 "config.y"
{newattr(CLOSE,yyvsp[0]);}
break;
case 21:
#line 136 "config.y"
{newattr(INIT,yyvsp[0]);}
break;
case 22:
#line 138 "config.y"
{newattr(GETC,yyvsp[0]);}
break;
case 23:
#line 140 "config.y"
{newattr(PUTC,yyvsp[0]);}
break;
case 24:
#line 142 "config.y"
{newattr(READ,yyvsp[0]);}
break;
case 25:
#line 144 "config.y"
{newattr(WRITE,yyvsp[0]);}
break;
case 26:
#line 146 "config.y"
{newattr(SEEK,yyvsp[0]);}
break;
case 27:
#line 148 "config.y"
{newattr(CNTL,yyvsp[0]);}
break;
case 28:
#line 151 "config.y"
{yyval = atoi(yytext,yyleng);}
break;
case 32:
#line 159 "config.y"
{mkdev(yyvsp[-3],yyvsp[-1],yyvsp[0]);}
break;
case 33:
#line 162 "config.y"
{yyval = currdname = ckdname(yyvsp[0]);}
break;
case 34:
#line 165 "config.y"
{yyval = 0;}
break;
case 35:
#line 167 "config.y"
{yyval = yyvsp[0];}
break;
#line 1007 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
