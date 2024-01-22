/*  DEBUG OPTIONS */
#include <conf.h>
#include <kernel.h>
#include <debug.h>
#include <network.h>

struct debug_type debug_opt[] = { DEBUG_OPTIONS };

#define USAGE	"Usage: set option [level]\n"
/*------------------------------------------------------------------------
 *  x_echo  -  (command echo) echo arguments separated by blanks
 *------------------------------------------------------------------------
 */
COMMAND	x_set(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	str[80];
	int	i;
	if (nargs == 1)
		{
           	for (i=0;i< (sizeof(debug_opt) / sizeof(struct debug_type));i++)
			{
			sprintf(str, "OPTION : %10s  LEVEL : %d\n", debug_opt[i].option, debug_opt[i].level);
			write(stdout, str, strlen(str));
			}
		write(stdout, USAGE, strlen(USAGE));
		return (OK);
		}
	else if (nargs == 2){
           for (i=0;i< (sizeof(debug_opt) / sizeof(struct debug_type));i++)
		if (strcmp(args[1], debug_opt[i].option) == 0) 
			{
			sprintf(str, "Set option : %s at level %d\n", 
				args[1], debug_opt[i].level);
			write(stdout, str, strlen(str));
			return (OK);
			}
	}
	else if (nargs == 3){
           for (i=0;i< (sizeof(debug_opt) / sizeof(struct debug_type));i++)
		if (strcmp(args[1], debug_opt[i].option) == 0) 
			{
			debug_opt[i].level = atoi(args[2]);
			sprintf(str, "Set option : %s at level %d\n", 
				args[1], debug_opt[i].level);
			write(stdout, str, strlen(str));
			if (debug_level("mcast"))
				{
				struct etdev *ped;

			    ped = &ee[0];  
				ped->ed_mcset = 1;
				ee_wstrt(ped);
 		    	debug_opt[i].level = 0;
				}
			}
	}
	return(OK);
}

int debug_level(char *option)
{
	int i;
    for (i=0;i< (sizeof(debug_opt) / sizeof(struct debug_type));i++)
		if (strcmp(option, debug_opt[i].option) == 0) 
				return debug_opt[i].level;
	return 0;
}
