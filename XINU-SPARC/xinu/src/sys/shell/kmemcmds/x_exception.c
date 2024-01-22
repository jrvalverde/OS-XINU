/* x_exception.c - x_exception */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <vmem.h>


/* local routines */
static writeto();
static readfrom();
static divzero();
static illegal();
static jumpto();
static recurse();
static Log();
static Usage();

static int LogDev;



/*------------------------------------------------------------------------
 *  x_exception - generate exceptions
 *------------------------------------------------------------------------
 */
COMMAND	x_exception(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int i;
	int addr;

	LogDev = stderr;

	if (nargs != 2) {
		Usage(args[0]);
		return(-1);
	}


	/* parse the arguments */
	for (i=1; i < nargs; ++i) {
		if (*args[i] == 'r') {
			sscanf(args[i]+1,"%x",&addr);
			readfrom(addr);
		} else if (*args[i] == 'w') {
			sscanf(args[i]+1,"%x",&addr);
			writeto(addr,0xf0f0f0f0);
		} else if (*args[i] == 'j') {
			sscanf(args[i]+1,"%x",&addr);
			jumpto(addr);
		} else if (*args[i] == 'i') {
			sscanf(args[i]+1,"%x",&addr);
			illegal(addr);
		} else if (strcmp(args[i],"z") == 0) {
			divzero();
		} else if (strcmp(args[i],"s") == 0) {
			recurse();
		} else
		    Usage();
	}
	return(OK);
}


static
writeto(addr,val)
     int *addr;
     int val;
{
	Log("Trying to write 0x%x to address 0x%08lx\n\n", val, addr);
	sleep(1);

	*addr = val;
}


static
readfrom(addr)
     int *addr;
{
	int value;

	Log("Trying to read from address 0x%08lx\n\n", addr);
	sleep(1);

	value = *addr;
	Log("Value is 0x%08lx\n", value);
}

static
jumpto(addr)
     void (*addr)();
{
	Log("Trying to jump to address 0x%08lx\n\n", addr);
	sleep(1);

	(*addr)();

	Log("returned\n");
}



static
divzero()
{
	int i,d;

	Log("Trying to divide by zero\n\n");
	sleep(1);

	/* yeesch, the compiler is too smart for us!!! */
	d = 0;
	for (i = -2; i < 2; ++i)
	    d += 10 / i;
}


static
illegal(instr)
     unsigned instr;
{
	void (*ill_func)();

	Log("Trying to execute illegal instruction (0x%08x)\n", instr);
	sleep(1);

	ill_func = (void (*)()) &instr;

	(*ill_func)();
}


static
recurse()
{
	char filler[0x2000];
	int i;

	i = 10;
	return(recurse());
}



/*VARARGS1*/
static
Log(m,a1,a2,a3,a4)
     char *m;
     int a1,a2,a3,a4;
{
    fprintf(LogDev,m,a1,a2,a3,a4);
}



static
Usage(prog)
     char *prog;
{
	Log("usage: %s rHEX | wHEX | jHEX | iHEX | z \n", prog);
	Log("   j addr    jump to address 'addr'\n");
	Log("   r addr    read from address 'addr'\n");
	Log("   w addr    write to address 'addr'\n");
	Log("   i instr   execute an illegal instruction\n");
	Log("   z         generate a divide by 0\n");
	Log("   s         stack overflow - recursion\n");
	Log(" (addresses should be in hex)\n");
}
