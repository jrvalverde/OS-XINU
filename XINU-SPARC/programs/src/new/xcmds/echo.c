/* echo.c - echo */

#define UNIX_STDOUT	1

/*------------------------------------------------------------------------
 *  echo  -  (command echo) echo arguments separated by blanks
 *------------------------------------------------------------------------
 */
main(nargs, args)
int	nargs;
char	*args[];
{
	char	str[80];
	int	i;

	if (nargs == 1)
		str[0] = '\0';
	else {
		for (strcpy(str, args[1]),i=2 ; i<nargs ; i++) {
			strcat(str, " ");
			strcat(str, args[i]);
		}
	}
	strcat(str, "\n");
	write(UNIX_STDOUT, str, strlen(str));
}
