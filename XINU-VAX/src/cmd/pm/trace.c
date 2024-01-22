#include "pm.h"

/*
 * pm - traceback routines
 */


struct nlist	symbol;
INT		lastframe;
INT		lastpc;
INT		cntflg;
INT		cntval;

/* symbol management */
L_INT		localval;

struct reglist reglist [] = {
		"psl", PSL,
		"pc", PC,
		"fp", FP,
		"ap", AP,
		"r11", R11,
		"r10", R10,
		"r9", R9,
		"r8", R8,
		"r7", R7,
		"r6", R6,
		"r5", R5,
		"r4", R4,
		"r3", R3,
		"r2", R2,
		"r1", R1,
		"r0", R0,
		"sp", KSP,
		"usp", USP,
		"ssp", SSP,
		"esp", ESP,
		"p0br", P0BR,
		"p0lr", P0LR,
		"p1br", P1BR,
		"p1lr", P1LR,
};

char		lastc;
extern	struct	pentry	*proctb;


/* general printing routines  */

printtrace(regs, base, pid)
unsigned INT	regs[REGISTERS];
unsigned INT	base;
unsigned INT	pid;
{
	unsigned	INT	currpc;
	int		narg, rcnt;
	unsigned INT	frame, argp;
	unsigned INT	indx;
	
	frame = regs[FP] & EVEN;
	argp = regs[AP] & EVEN;
	currpc = regs[PC];
	rcnt = 0;	/* recursion count limit */
	while(1){
		if(core[(frame+12)>>2] == INITREG) { /* head of Xinu task */
			findsym(proctb[pid].paddr, N_TEXT);
			if (symbol.n_un.n_strx)
				printf("%s(", strtab+symbol.n_un.n_strx);
			narg = proctb[pid].pargs;
			if(--narg >= 0)
				printf("0x%x", core[argp>>2]);
			while(--narg >= 0){
				argp += 4;
				printf(", 0x%x", core[argp>>2]);
			}
			printf(")");
			if(narg > 0)
				printf(" plus %d possible argument%s",
						narg, narg==1?"":"s");
			
			return;
		}
		if(rcnt > RECLIMIT){
			printf("recursion limit of %d reached\n", RECLIMIT);
			return;
		}
		narg = findroutine(currpc, argp); /* fills in symbol */
		if(errflg){
			printf("non-C function: ");
			rcnt++;
		}
		if (symbol.n_un.n_strx)
			printf("%s(", strtab+symbol.n_un.n_strx);
		else
			printf("(");
		argp += 4;
		if(--narg >= 0)
			printf("0x%x", core[argp>>2]);
		while(--narg >= 0){
			argp += 4;
			printf(", 0x%x", core[argp>>2]);
		}
		printf(")");
		if(narg > 0)
			printf(" plus %d possible argument%s",
					narg, narg==1?"":"s");
		printf("\n");

		if (frame >= (base - 4))
			break;

		/* obtain return address; move frame ptr to previous frame */

		currpc = core[(frame+16)>>2];
		argp = core[(frame+8)>>2];
		frame = core[(frame+12)>>2];
		if (frame < proctb[pid].plimit
			|| frame >= proctb[pid].pbase) {
			printf("...possible stack corruption\n");
			break;
		}
	}
}

printregs(regs)
unsigned INT	regs[];
{
	register struct reglist *p;
	INT			v;


	for(p = reglist; p < &reglist[REGISTERS]; p++){
		v = regs[p->roffs];
		printf("%s\t0x%x\t\t", p->rname, v);
		if (p->roffs <= PC)
			valpr((INT)v,(p->roffs == PC?N_TEXT:N_DATA));
		printf("\n");
	}
}

/*
 * Print a value v symbolically, if it has a reasonable
 * interpretation as name+offset. If not, print nothing.
 */

valpr(v, idsp)
unsigned INT	v;
{
	long	findsym(), d;

	d = findsym(v, idsp);
	if(d >= MAXOFF)
		return;
	if (symbol.n_un.n_strx) {
		printf("%s", strtab+symbol.n_un.n_strx);
		if(d)
			printf("+0x%x", d);
	}
}

/*
 * printpc - print the word pointed to by the pc both
 * symbolically and as an instruction
 */

printpc(regs)
INT	regs[REGISTERS];
{
	psymoff(regs[PC], ":\t", N_TEXT);
/*	printins(regs[PC], regs);*/
}

/*
 * findroutine - find the name of the routine pointed
 * to by the current frame. Leave its symbol table entry in
 * symbol as a side effect; returns the number of arguments 
 * used in the call.
 *
 */

findroutine(currpc, argp)
unsigned INT	argp;
unsigned INT	currpc;
{
	int	nargs;

	errflg = FALSE;
	if(findsym(currpc, N_TEXT) == -1) {
		errflg = TRUE;
		symbol.n_un.n_name = NULL;
		symbol.n_value = 0;
	}
	nargs = core[argp>>2] & 0xff;
	return(nargs);
}

/*
 * find the closest symbol to val, and return the 
 * difference between val and the symbol found.
 * leave the symbol table entry in 'symbol' as a side effect.
 */

long
findsym(val, type)
INT	val;
INT	type;
{
	long			diff;
	register struct nlist	*sp, *cursym;

	diff = 0377777L;
	cursym = &symbol;
	symbol.n_un.n_name = NULL;
	symbol.n_value = 0;

	for(sp = ssymtab; sp < essymtab; sp++){
		switch(type){
		case N_TEXT:
			if((sp->n_type & N_TYPE) != type)
				continue;
			break;

		case N_DATA:
			if((sp->n_type & N_TYPE) != N_DATA 
			   && (sp->n_type & N_TYPE) != N_BSS)
				break;
			continue;

		default:
			fprintf(stderr, "bad type in findsym\n");
		}
		if(sp->n_value <= val && sp->n_value > cursym->n_value)
			cursym = sp;
	}
	if(cursym->n_un.n_name)
		symbol = *cursym;
	else
		symbol.n_un.n_name = NULL;
	return(val - symbol.n_value);
}

/*
 * psymoff - basically call findsym and print the result
 */

psymoff(val, str, space)
INT	val;
char	*str;
{
	long findsym(), d;

	d = findsym(val, space);
	if(d > MAXOFF)
		printf("0x%x", val);
	else{
		if (symbol.n_un.n_strx)
			printf("%s", strtab+symbol.n_un.n_strx);
		if(d)
			printf("+0x%x", d);
	}
	printf(str);
}
