/* 
 * command.h
 * 
 * Author:	Victor Norman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Nov 16 13:28:41 1988
 *
 * Copyright (c) 1988 Victor Norman
 */


struct builtin {
	char 	*name;
	int	value;
	char	*usage;
};

/* must be unique and non-negative */
#define SET 		0
#define UNSET 		1
#define SETENV		2
#define UNSETENV 	3
#define EXPORT		4
#define CD		5
#define SOURCE		6
#define BYE		7
#define PRINT		8
#define PRINTENV	9
#define PWD		10
#define ALIAS		11
#define UNALIAS		12
#define HELP		13
#define XSH_DEBUG	14

#ifdef BUILTIN_ECHO
#define ECHO		15
#endif

#define NUMBUILTINS (sizeof(BuiltIn) / sizeof(struct builtin))
