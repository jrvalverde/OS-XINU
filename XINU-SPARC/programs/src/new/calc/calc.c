
/* 
 * calc.c - A very simple command line calculator
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Jul 18 15:05:57 1990
 *
 * Copyright (c) 1990 Jim Griffioen
 */

#include <stdio.h> /* I/O library */


main (argc,argv)
int argc;
char *argv[];
{
  int ii;
  int num;
  int total;
  char *str;
  int plus,multiply,div,mod,sub;


  if (argc == 1) {
	  fprintf(stderr, "usage: %s operand op eqn\n", argv[0]);
	  fprintf(stderr, "  where: operand     is an integer\n");
	  fprintf(stderr, "         op          is + - / x\n");
	  fprintf(stderr, "         eqn         is an operand\n");
	  fprintf(stderr, "                 or\n");
	  fprintf(stderr, "         eqn         is operand op operand\n");
	  fprintf(stderr, "  evaluation is always left to right\n");
	  exit(1);
  }
  total = 0;
  plus = 1;
  multiply = 0;
  div = 0;
  mod = 0;
  sub = 0;
  for (ii=1;ii<=argc-1;ii++)
    {
    num = 0;
    if (0 == strcmp(argv[ii],"+")) 
       plus = 1;
    else if (0 == strcmp(argv[ii],"-"))
       sub= 1;
    else if (0 == strcmp(argv[ii],"x"))
       multiply = 1;
    else if (0 == strcmp(argv[ii],"/"))
       div = 1;
    else if (0 == strcmp(argv[ii],"%"))
       mod = 1;
    else 
       {
       num = atoi(argv[ii]);
       if (plus) total = total + num;
       else if (sub) total = total - num;
       else if (multiply) 
          total = total * num;
       else if (div) total = total / num;
       plus = 0;
       multiply = 0;
       div = 0;
       mod = 0;
       sub = 0;
       }
    }
    printf("calc: answer = %d \n",total);
}


