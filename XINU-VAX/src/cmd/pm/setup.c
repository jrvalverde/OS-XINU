#include "pm.h"
#include <sys/stat.h>

/*
 * pm - initialization routines
 */

struct	nlist	nl[8];		/* symbols pulled out of the text image	 */
#define	CURRPID		0
#define	PROC		1
#define	XQ		2
#define	SEMS		3
#define	TTYS		4
#define	VERS		5
#define	CLKTIM		6

extern	struct	pentry	*proctb;
extern	struct	sentry	*semtab;
extern	struct	tty	*tptr;
extern	struct	qent	*Q;
extern	char	*vers;
extern	long	clktim;

setup(argc, argv)
int	argc;
char	*argv[];
{
	char		*malloc();
	FILE		*txtfd, *corefd;
	struct stat	stat;
	register struct nlist	*sp1, *sp2;
	register 	i;
	int		s1, s2, strtabsize;
	int		svalcomp();

	nl[0].n_un.n_name = "_currpid";
	nl[1].n_un.n_name = "_proctab";
	nl[2].n_un.n_name = "_q";
	nl[3].n_un.n_name = "_semaph";
	nl[4].n_un.n_name = "_tty";
	nl[5].n_un.n_name = "_vers";
	nl[6].n_un.n_name = "_clktime";
	nl[7].n_un.n_name = NULL;

	procargs(argc, argv);

	if (verbose)
		printf("Opening core file...\n");
	corefd = fopen(corefile, "r");
	if(corefd == (FILE *)NULL){
		fprintf(stderr, "Can't open %s\n", corefile);
		exit(-1);
	}
	if (verbose)
		printf("Opening text file...\n");
	txtfd = fopen(txtfile, "r");
	if(txtfd == (FILE *)NULL){
		fprintf(stderr, "Can't open %s\n", txtfile);
		exit(-2);
	}
	if (verbose)
		printf("Reading core header of %d bytes\n",
			sizeof(struct coreX) );
	fread(&c_header, sizeof(struct coreX), 1, corefd);
	fstat(fileno(corefd), &stat);
	if ( stat.st_size <= sizeof(struct coreX) ) {
		fprintf(stderr,"Malformed core file header (size=%d)\n",
			stat.st_size);
		exit(-2);
	}
	stat.st_size -= sizeof(struct coreX);
	if (verbose)
		printf("Allocating %d bytes for core image\n",stat.st_size);
	core = (int *)malloc(stat.st_size);
	if(core == (int *)NULL){
		fprintf(stderr, "Can't allocate memory for core image\n");
		exit(-3);
	}
	if (verbose)
		printf("Reading core image...\n");
	fread(core, stat.st_size, 1, corefd);
	if (verbose)
		printf("Reading a.out header...\n");
	fread(&a_out, sizeof(struct exec), 1, txtfd);
	if (N_BADMAG(a_out)) {
		fprintf(stderr,"bad magic number in %s\n", txtfile);
		exit(2);
	}
	text = (int *)malloc(a_out.a_text + a_out.a_data + a_out.a_bss);
	if(text == (int *)NULL){
		fprintf(stderr, "Can't allocate space for system image\n");
		exit(-4);
	}
	fseek(txtfd, N_TXTOFF(a_out), 0);
	fread(text, a_out.a_text + a_out.a_data, 1, txtfd);

	if (N_STROFF(a_out) < N_TXTOFF(a_out)) {
		fprintf(stderr,"stripped a.out file\n");
		exit(2);
	}
	fseek(txtfd, N_STROFF(a_out), 0);
	fread(&strtabsize, sizeof(int), 1, txtfd);	/* size of str tab */
	fseek(txtfd, N_STROFF(a_out), 0);
	strtab = malloc(strtabsize);			/* get string table */
	fread(strtab, strtabsize, 1, txtfd);

	symtab = (struct nlist *)malloc(a_out.a_syms);
	esymtab = &symtab[a_out.a_syms / sizeof (struct nlist)];
	if(a_out.a_syms == 0)
	       	printf("No symbol table\n");
	if(symtab == (struct nlist *)NULL){
		fprintf(stderr, "Can't allocate space for symbol table\n");
		exit(-5);
	}
	fseek(txtfd, N_SYMOFF(a_out), 0);
	fread(symtab, a_out.a_syms, 1, txtfd);
	
	/*
	 * produce list of externals
	 */

	ssymtab = (struct nlist *)malloc(a_out.a_syms);
	i = 0;
	for(sp1 = symtab, sp2 = ssymtab; sp1 < esymtab; sp1++){
		if((sp1->n_type & N_EXT) == 0)
			continue;
		i++;
		*sp2++ = *sp1;
	}

	essymtab = ssymtab + i;

	/* 
	 * look up addresses of interesting tables 
	 */

	nlist(nl);
	currpid = core[nl[CURRPID].n_value >> 2];
	proctb = (struct pentry *)(&core[nl[PROC].n_value>>2]);
	semtab = (struct sentry *)(&core[nl[SEMS].n_value>>2]);
	Q = (struct qent *)(&core[nl[XQ].n_value >> 2]);
        tptr = (struct tty *)(&core[nl[TTYS].n_value >> 2]);
	vers = nl[VERS].n_value == 0 ? "6" :
		(char *)(&core[nl[VERS].n_value >> 2]);
	if (nl[CLKTIM].n_value == 0) {
		clktim = 0L;
	} else
		clktim = (*((long *) &core[(nl[CLKTIM].n_value>>2)]));
}

svalcomp(sp1, sp2)
register struct nlist *sp1, *sp2;
{
	if(sp1->n_value == sp2->n_value)
		return(0);
	if(sp1->n_value < sp2->n_value)
		return(-1);
	return(1);
}

#define SPACE 100		/* number of symbols read at a time */

nlist(list)
struct nlist *list;
{
	register struct nlist *p, *q;

	for(p = list; p->n_un.n_name; p++) {
		p->n_type = 0;
		p->n_value = 0;
	}
	for(q = ssymtab; q <= essymtab; q++) {
		for(p = list; p->n_un.n_name; p++) {
			if (q->n_un.n_strx) {
			    if (strcmp(strtab+q->n_un.n_strx,
			               p->n_un.n_name) != 0)
				goto cont;
			    p->n_value = q->n_value;
			    p->n_type = q->n_type;
			    break;
			}
			cont:		;
		}
	}
	return(0);
}

/*
 *===================================================
 * procargs - unified argument processing procedure
 *===================================================
 *
 * This procedure continas the logic for converting the UNIX argument
 * list into global variables
 */

procargs(argc, argv)
int argc;
char *argv[];
{
        int arg, unswitched, more;
        char *swptr;

	corefile = "coreX";
	txtfile = "a.out";
	allopts = 1;
	popt = sopt = topt = 0;
	verbose = 0;

        unswitched = 0;
        for ( arg=1 ; arg<argc ; arg++ ) {
                if ( argv[arg][0] == '-' ) {
                        more = 1;
                        swptr = &argv[arg][1];
                        while ( more && *swptr!='\0' ) {
                                switch ( *swptr++ ) {
				case 'v':
					verbose=1;

				case 'p':
					allopts = 0;
					popt = 1;
					break;

				case 's':
					allopts = 0;
					sopt = 1;
					break;

				case 't':
					allopts = 0;
					topt = 1;
					break;

                                default:
                                        usagexit(argv[0]);
                                }
                        }
                } else { /* there's no dash in front */
                        switch ( unswitched++ ) {
			case 0:
				txtfile = argv[arg];
				break;
			
			case 1:
				corefile = argv[arg];
				break;

                        default:
                                usagexit(argv[0]);
                        }
                }
        }
}
usagexit(pgm)
char *pgm;
{
        fprintf(stderr,"usage: %s [program [core]]\n",pgm);
        exit(1);
}
