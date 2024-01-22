
#ifdef XINU8
#ifndef NO_DIR
#define NULL	0

/*
 * getwd() - return the current working directory
 */
char *getwd()
{
    register char *dir;

    if ((dir = (char *) getenv("PWD")) == NULL)
	dir = "";
    return(dir);
}
#endif
#endif
