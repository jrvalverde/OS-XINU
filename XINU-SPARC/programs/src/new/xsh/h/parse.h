
/* 
 * parse.h - parse header file
 * 
 * Author:	Victor Norman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Nov 18 13:56:16 1988
 *
 * Copyright (c) 1988 Victor Norman
 */
struct	arglist {
	char	*arg;
	int	type;
	struct	arglist *next;
};

struct cmd_rec {
	struct arglist	*args;
	struct {
		int type;
		char *value;
	} cmd, input, output;
	int	backgrounding;
	struct cmd_rec *next;
};

