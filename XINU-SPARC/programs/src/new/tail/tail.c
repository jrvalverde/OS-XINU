/* tail -- output last part of file(s)
   Copyright (C) 1989 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* Can display any amount of data, unlike the Unix version, which uses
   a fixed size buffer and therefore can only deliver a limited number
   of lines.

   Started by Paul Rubin <phr@ai.mit.edu>
   Finished by David MacKenzie <djm@ai.mit.edu>

   Usage: tail [-n [+]#] [-lbcfqv] [+number [+]#] [+lines] [+blocks]
          [+chars] [+follow] [+quiet] [+silent] [+verbose] [file...]

          tail [+/-#lbcfqv] [file...]

   Options:
   -n, +number [+]#	Number of items to tail (default 10).
			If the number starts with a `+', begin printing with
			the #th item from the start of each file, instead of
			from the end.
   -l, +lines		Tail by lines (the default).
   -b, +blocks		Tail by 512-byte blocks.
   -c, +chars		Tail by characters.
   -f, +follow		Loop forever trying to read more characters at the
			end of the file, on the assumption that the file
			is growing.  Ignored if reading from a pipe.
			Cannot be used if more than one file is given.
   -q, +quiet, +silent	Never print filename headers.
   -v, +verbose		Always print filename headers.

   Reads from standard input if no files are given or when a filename of
   ``-'' is encountered.
   By default, filename headers are printed only more than one file
   is given.
   Compile with -DUSG under System V. */

#ifdef XINU8
#define xwrite	gwrite
#endif

#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#define ISDIGIT(c) (isascii (c) && isdigit (c))
#include "getopt.h"
#include "system.h"

/* Number of items to tail. */
#define DEFAULT_NUMBER 10

#define BLOCKSIZE 512

/* Size of atomic reads. */
#define BUFSIZE (BLOCKSIZE*8)

/* Masks for the operation mode.  If neither CHARS nor BLOCKS is set,
   tail operates by lines. */
#define CHARS 1			/* Tail by characters. */
#define BLOCKS 2		/* Tail by blocks. */
#define FOREVER 4		/* Read from end of file forever. */
#define START 8			/* Count from start of file instead of end. */
#define HEADERS 16		/* Print filename headers. */

/* When to print the filename banners. */
enum header_mode
{
  multiple_files, always, never
};

char *index ();
char *malloc ();
char *rindex ();
char *strcat ();
char *strcpy ();
void free ();

char *basename ();
char *xmalloc ();
int file_lines ();
int start_chars ();
int start_lines ();
void dump_remainder ();
void fatal_perror ();
void nonfatal_perror ();
void pipe_chars ();
void pipe_lines ();
void tail ();
void tail_chars ();
void tail_file ();
void tail_lines ();
void usage ();
void write_header ();
void xwrite ();

/* The base of the name this program was run with. */
char *program_name;

/* The exit status; 1 if an error occurred, 0 otherwise. */
int errors;

struct option long_options[] =
{
  {"number", 1, NULL, 'n'},
  {"lines", 0, NULL, 'l'},
  {"blocks", 0, NULL, 'b'},
  {"chars", 0, NULL, 'c'},
  {"follow", 0, NULL, 'f'},
  {"quiet", 0, NULL, 'q'},
  {"silent", 0, NULL, 'q'},
  {"verbose", 0, NULL, 'v'},
  {NULL, 0, NULL, 0}
};

int
main (argc, argv)
     int argc;
     char **argv;
{
  enum header_mode header_mode = multiple_files;
  int mode = 0;			/* Flags. */
  /* In START mode, the number of items to skip before printing; otherwise,
     the number of items at the end of the file to print.  Initially, -1
     means the value has not been set. */
  int number = -1;
  int c;			/* Option character. */
  int longind;			/* Index in `long_options' of option found. */

  program_name = argv[0] = basename (argv[0]);
  errors = 0;

  if (argc > 1
      && ((argv[1][0] == '-' && ISDIGIT (argv[1][1]))
	  || (argv[1][0] == '+' && (ISDIGIT (argv[1][1]) || argv[1][1] == 0))))
    {
      /* Old option syntax: a dash or plus, one or more digits, and one or
	 more option letters. */
      if (argv[1][0] == '+')
	mode |= START;
      if (ISDIGIT (argv[1][1]))
	{
	  for (number = 0, ++argv[1]; ISDIGIT (*argv[1]); ++argv[1])
	    number = number * 10 + *argv[1] - '0';
	  /* Parse any appended option letters with getopt. */
	  if (*argv[1])
	    *--argv[1] = '-';
	  else
	    argv[1] = "-l";
	}
      else
	argv[1] = "-l";
    }
  while ((c = getopt_long (argc, argv, "n:lbcfqv", long_options, &longind))
	 != EOF)
    {
      if (c == 0)
	c = long_options[longind].val;
      switch (c)
	{
	case 'n':
	  if (*optarg == '+')
	    {
	      mode |= START;
	      ++optarg;
	    }
	  number = atou (optarg);
	  if (number == -1)
	    {
	      fprintf (stderr, "%s: invalid number `%s'\n",
		       program_name, optarg);
	      exit (1);
	    }
	  break;
	case 'l':
	  mode &= ~(CHARS | BLOCKS);
	  break;
	case 'b':
	  mode |= BLOCKS;
	  mode &= ~CHARS;
	  break;
	case 'c':
	  mode |= CHARS;
	  mode &= ~BLOCKS;
	  break;
	case 'f':
	  mode |= FOREVER;
	  break;
	case 'q':
	  header_mode = never;
	  break;
	case 'v':
	  header_mode = always;
	  break;
	default:
	  usage ();
	}
    }

  if (number == -1)
    number = DEFAULT_NUMBER;

  /* To start printing with item `number' from the start of the file, skip
     `number' - 1 items.  `tail -0s' is actually meaningless, but for Unix
     compatibility it's treated the same as `tail -1s'. */
  if (mode & START)
    {
      if (number)
	--number;
    }

  if (mode & BLOCKS)
    number *= BLOCKSIZE;

  if (optind < argc - 1 && (mode & FOREVER))
    {
      fprintf (stderr, "%s: cannot follow the ends of multiple files\n",
	       program_name);
      exit (1);
    }

  if (header_mode == always
      || header_mode == multiple_files && optind < argc - 1)
    mode |= HEADERS;

  if (optind == argc)
    tail_file ("-", mode, number);

  for (; optind < argc; ++optind)
    tail_file (argv[optind], mode, number);

  exit (errors);
}

void
tail_file (filename, mode, number)
     char *filename;
     int mode;
     int number;
{
  int fd;

  if (!strcmp (filename, "-"))
    {
      filename = "standard input";
      if (mode & HEADERS)
	write_header (filename);
      tail (filename, 0, mode, number);
    }
  else
    {
      fd = open (filename, O_RDONLY);
      if (fd == -1)
	nonfatal_perror ("cannot open", filename);
      else
	{
	  if (mode & HEADERS)
	    write_header (filename);
	  tail (filename, fd, mode, number);
	  close (fd);
	}
    }
}

void
write_header (filename)
     char *filename;
{
  static int first_file = 1;

  if (first_file)
    {
      xwrite (1, "==> ", 4);
      first_file = 0;
    }
  else
    xwrite (1, "\n==> ", 5);
  xwrite (1, filename, strlen (filename));
  xwrite (1, " <==\n", 5);
}

void
tail (filename, fd, mode, number)
     char *filename;
     int fd;
     int mode;
     int number;
{
  if (mode & (CHARS | BLOCKS))
    tail_chars (filename, fd, mode, number);
  else
    tail_lines (filename, fd, mode, number);
}

void
tail_chars (filename, fd, mode, number)
     char *filename;
     int fd;
     int mode;
     int number;
{
  int length;

  if (mode & START)
    {
      if (lseek (fd, number, L_SET) < 0)
	{
	  /* Reading from a pipe. */
	  mode &= ~FOREVER;
	  if (start_chars (filename, fd, number) == 0)
	    return;
	}
      dump_remainder (filename, fd, mode);
    }
  else
    {
#ifdef XINU8
      length = getsize(filename);
#else
      length = lseek (fd, 0, L_XTND);
#endif
      if (length >= 0)
	{
	  if (length <= number)
	    /* The file is shorter than we want, or just the right size, so
	       print the whole file. */
	    lseek (fd, 0, L_SET);
	  else
	    /* The file is longer than we want, so go back. */
#ifdef XINU8
	    lseek (fd, length-number, L_SET);
#else
	    lseek (fd, -number, L_XTND);
#endif
	  dump_remainder (filename, fd, mode);
	}
      else
	pipe_chars (filename, fd, number);
    }
}

void
tail_lines (filename, fd, mode, number)
     char *filename;
     int fd;
     int mode;
     int number;
{
  int length;

  if (mode & START)
    {
      if (lseek (fd, 0, L_SET) < 0)
	mode &= ~FOREVER;
      if (start_lines (filename, fd, number) == 0)
	return;
      dump_remainder (filename, fd, mode);
    }
  else
    {
#ifdef XINU8
      length = getsize(filename);
#else
      length = lseek (fd, 0, L_XTND);
#endif
      if (length >= 0)
	{
	  if (length && file_lines (filename, fd, number, length) == 0)
	    return;
	  dump_remainder (filename, fd, mode);
	}
      else
	pipe_lines (filename, fd, number);
    }
}

/* Print the last `number' lines from the end of file `fd'.
   Go backward through the file, reading `BUFSIZE' bytes at a time (except
   probably the first), until we hit the start of the file or have
   read `number' newlines.
   `pos' starts out as the length of the file (the offset of the last
   byte of the file + 1).
   Return 0 on error, 1 if ok.  */

int
file_lines (filename, fd, number, pos)
     char *filename;
     int fd;
     int number;
     int pos;
{
  char buffer[BUFSIZE];
  int chars_read;
  int i;			/* Index into `buffer' for scanning. */

  if (number == 0)
    return 1;

  /** Set `chars_read' to the size of the last, probably partial, buffer;
     0 < `chars_read' <= `BUFSIZE'. */
  chars_read = pos % BUFSIZE;
  if (chars_read == 0)
    chars_read = BUFSIZE;
  /* Make `pos' a multiple of `BUFSIZE' (0 if the file is short), so that all
     reads will be on block boundaries, which might increase efficiency. */
  pos -= chars_read;
  lseek (fd, pos, L_SET);
  chars_read = read (fd, buffer, chars_read);
  if (chars_read == -1)
    {
      nonfatal_perror ("error reading", filename);
      return 0;
    }

  /* Count the incomplete line on files that don't end with a newline. */
  if (chars_read && buffer[chars_read - 1] != '\n')
    --number;

  do
    {
      /* Scan backward, counting the newlines in this bufferfull. */
      for (i = chars_read - 1; i >= 0; i--)
	{
	  /* Have we counted the requested number of newlines yet? */
	  if (buffer[i] == '\n' && number-- == 0)
	    {
	      /* If this newline wasn't the last character in the buffer,
	         print the text after it. */
	      if (i != chars_read - 1)
		xwrite (1, &buffer[i + 1], chars_read - (i + 1));
	      return 1;
	    }
	}
      /* Not enough newlines in that bufferfull. */
      if (pos == 0)
	{
	  /* Not enough lines in the file; print the entire file. */
	  lseek (fd, 0, L_SET);
	  return 1;
	}
      pos -= BUFSIZE;
      lseek (fd, pos, L_SET);
    }
  while ((chars_read = read (fd, buffer, BUFSIZE)) > 0);
  if (chars_read == -1)
    {
      nonfatal_perror ("error reading", filename);
      return 0;
    }
  return 1;
}

/* Print the last `number' lines from the end of pipe `fd'.
   Buffer the text as a linked list of LBUFFERs, adding them as needed. */

void
pipe_lines (filename, fd, number)
     char *filename;
     int fd;
     int number;
{
  struct linebuffer
  {
    int nchars, nlines;
    char buffer[BUFSIZE];
    struct linebuffer *next;
  };
  typedef struct linebuffer LBUFFER;
  LBUFFER *first, *last, *tmp;
  int i;			/* Index into buffers. */
  int total_lines = 0;		/* Total number of newlines in all buffers. */

  first = last = (LBUFFER *) xmalloc (sizeof (LBUFFER));
  first->nchars = first->nlines = 0;
  tmp = (LBUFFER *) xmalloc (sizeof (LBUFFER));

  /* Input is always read into a fresh buffer. */
  while ((tmp->nchars = read (fd, tmp->buffer, BUFSIZE)) > 0)
    {
      tmp->nlines = 0;
      tmp->next = NULL;

      /* Count the number of newlines just read. */
      for (i = 0; i < tmp->nchars; i++)
	if (tmp->buffer[i] == '\n')
	  ++tmp->nlines;
      total_lines += tmp->nlines;

      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, `nchars' can
         often be very small. */
      if (tmp->nchars + last->nchars < BUFSIZE)
	{
	  bcopy (tmp->buffer, &last->buffer[last->nchars], tmp->nchars);
	  last->nchars += tmp->nchars;
	  last->nlines += tmp->nlines;
	}
      else
	{
	  /* If there's not enough room, link the new buffer onto the end of
	     the list, then either free up the oldest buffer for the next
	     read if that would leave enough lines, or else malloc a new one.
	     Some compaction mechanism is possible but probably not
	     worthwhile. */
	  last = last->next = tmp;
	  if (total_lines - first->nlines > number)
	    {
	      tmp = first;
	      total_lines -= first->nlines;
	      first = first->next;
	    }
	  else
	    tmp = (LBUFFER *) xmalloc (sizeof (LBUFFER));
	}
    }
  if (tmp->nchars == -1)
    nonfatal_perror ("error reading", filename);

  free ((char *) tmp);

  /* This prevents a core dump when the pipe contains no newlines. */
  if (number == 0)
    goto free_lbuffers;

  /* Count the incomplete line on files that don't end with a newline. */
  if (last->buffer[last->nchars - 1] != '\n')
    {
      ++last->nlines;
      ++total_lines;
    }

  /* Run through the list, printing lines.  First, skip over unneeded
     buffers. */
  for (tmp = first; total_lines - tmp->nlines > number; tmp = tmp->next)
    total_lines -= tmp->nlines;

  /* Find the correct beginning, then print the rest of the file. */
  if (total_lines > number)
    {
      char *cp;

      /* Skip `total_lines' - `number' newlines.  We made sure that
         `total_lines' - `number' <= `tmp->nlines'. */
      cp = tmp->buffer;
      for (i = total_lines - number; i; --i)
	while (*cp++ != '\n')
	  /* Do nothing. */ ;
      i = cp - tmp->buffer;
    }
  else
    i = 0;
  xwrite (1, &tmp->buffer[i], tmp->nchars - i);

  for (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite (1, tmp->buffer, tmp->nchars);

free_lbuffers:
  while (first)
    {
      tmp = first->next;
      free ((char *) first);
      first = tmp;
    }
}

/* Print the last `number' characters from the end of pipe `fd'.
   This is a stripped down version of pipe_lines. */

void
pipe_chars (filename, fd, number)
     char *filename;
     int fd;
     int number;
{
  struct charbuffer
  {
    int nchars;
    char buffer[BUFSIZE];
    struct charbuffer *next;
  };
  typedef struct charbuffer CBUFFER;
  CBUFFER *first, *last, *tmp;
  int i;			/* Index into buffers. */
  int total_chars = 0;		/* Total characters in all buffers. */

  first = last = (CBUFFER *) xmalloc (sizeof (CBUFFER));
  first->nchars = 0;
  tmp = (CBUFFER *) xmalloc (sizeof (CBUFFER));

  /* Input is always read into a fresh buffer. */
  while ((tmp->nchars = read (fd, tmp->buffer, BUFSIZE)) > 0)
    {
      tmp->next = NULL;

      total_chars += tmp->nchars;
      /* If there is enough room in the last buffer read, just append the new
         one to it.  This is because when reading from a pipe, `nchars' can
         often be very small. */
      if (tmp->nchars + last->nchars < BUFSIZE)
	{
	  bcopy (tmp->buffer, &last->buffer[last->nchars], tmp->nchars);
	  last->nchars += tmp->nchars;
	}
      else
	{
	  /* If there's not enough room, link the new buffer onto the end of
	     the list, then either free up the oldest buffer for the next
	     read if that would leave enough characters, or else malloc a new
	     one.  Some compaction mechanism is possible but probably not
	     worthwhile. */
	  last = last->next = tmp;
	  if (total_chars - first->nchars > number)
	    {
	      tmp = first;
	      total_chars -= first->nchars;
	      first = first->next;
	    }
	  else
	    {
	      tmp = (CBUFFER *) xmalloc (sizeof (CBUFFER));
	    }
	}
    }
  if (tmp->nchars == -1)
    nonfatal_perror ("error reading", filename);

  free ((char *) tmp);

  /* Run through the list, printing characters.  First, skip over unneeded
     buffers. */
  for (tmp = first; total_chars - tmp->nchars > number; tmp = tmp->next)
    total_chars -= tmp->nchars;

  /* Find the correct beginning, then print the rest of the file.
     We made sure that `total_chars' - `number' <= `tmp->nchars'. */
  if (total_chars > number)
    i = total_chars - number;
  else
    i = 0;
  xwrite (1, &tmp->buffer[i], tmp->nchars - i);

  for (tmp = tmp->next; tmp; tmp = tmp->next)
    xwrite (1, tmp->buffer, tmp->nchars);

  while (first)
    {
      tmp = first->next;
      free ((char *) first);
      first = tmp;
    }
}

/* Skip `number' characters from the start of pipe `fd', and print
   any extra characters that were read beyond that.
   Return 0 on error, 1 if ok.  */

int
start_chars (filename, fd, number)
     char *filename;
     int fd;
     int number;
{
  char buffer[BUFSIZE];
  int chars_read = 0;

  while (number > 0 && (chars_read = read (fd, buffer, BUFSIZE)) > 0)
    number -= chars_read;
  if (chars_read == -1)
    {
      nonfatal_perror ("error reading", filename);
      return 0;
    }
  else if (number < 0)
    xwrite (1, &buffer[chars_read + number], -number);
  return 1;
}

/* Skip `number' lines at the start of file or pipe `fd', and print
   any extra characters that were read beyond that.
   Return 0 on error, 1 if ok.  */

int
start_lines (filename, fd, number)
     char *filename;
     int fd;
     int number;
{
  char buffer[BUFSIZE];
  int chars_read = 0;
  int chars_to_skip = 0;

  while (number && (chars_read = read (fd, buffer, BUFSIZE)) > 0)
    {
      chars_to_skip = 0;
      while (chars_to_skip < chars_read)
	if (buffer[chars_to_skip++] == '\n' && --number == 0)
	  break;
    }
  if (chars_read == -1)
    {
      nonfatal_perror ("error reading", filename);
      return 0;
    }
  else if (chars_to_skip < chars_read)
    xwrite (1, &buffer[chars_to_skip], chars_read - chars_to_skip);
  return 1;
}

void
dump_remainder (filename, fd, mode)
     char *filename;
     int fd;
     int mode;
{
  char buffer[BUFSIZE];
  int chars_read;

output:
  while ((chars_read = read (fd, buffer, BUFSIZE)) > 0)
    xwrite (1, buffer, chars_read);
  if (chars_read == -1)
    fatal_perror ("error reading", filename);
  if (mode & FOREVER)
    {
      sleep (1);
      goto output;
    }
}

/* Write plus error check. */

void
xwrite (fd, buffer, count)
     int fd;
     int count;
     char *buffer;
{
  fd = write (fd, buffer, count);
#ifndef XINU8
  if (len != count)
    fatal_perror ("write", "error");
#endif
}

/* Allocate memory dynamically, with error check. */

char *
xmalloc (size)
     int size;
{
  char *p;

  p = malloc ((unsigned) size);
  if (p == NULL)
    {
      fprintf (stderr, "%s: virtual memory exhausted\n", program_name);
      exit (1);
    }
  return p;
}

/* Convert `str', a string of ASCII digits, into an unsigned integer.
   Return -1 if `str' does not represent a valid unsigned integer. */

int
atou (str)
     char *str;
{
  int value;

  for (value = 0; ISDIGIT (*str); ++str)
    value = value * 10 + *str - '0';
  return *str ? -1 : value;
}

void
nonfatal_perror (s1, s2)
     char *s1, *s2;
{
  fprintf (stderr, "%s: %s ", program_name, s1);
  perror (s2);
  errors = 1;
}

void
fatal_perror (s1, s2)
     char *s1, *s2;
{
  nonfatal_perror (s1, s2);
  exit (1);
}

/* Return `name' with any leading path stripped off.  */

char *
basename (name)
     char *name;
{
  char *base;

  base = rindex (name, '/');
  return base ? base + 1 : name;
}

void
usage ()
{
  fprintf (stderr, "\
Usage: %s [-n [+]#] [-lbcfqv] [+number [+]#] [+lines] [+blocks]\n\
       [+chars] [+follow] [+quiet] [+silent] [+verbose] [file...]\n\
\n\
       %s [+/-#lbcfqv] [file...]\n", program_name, program_name);
  exit (1);
}

#ifdef XINU8
int getsize(name)
     char *name;
{
    register int retval = 0;
    register int fd;
    register int len;
    register char buf[512];

    fd = open(name, O_RDONLY);
    if (fd < 0) {
	return(-1);
    }

    do {
	retval += 4096;
	lseek(fd, retval, L_SET);
    } while (read(fd, buf, 1) == 1);

    /* back up, we went too far */
    retval -= 4096;
    lseek(fd, retval, L_SET);

    /* just read in the last of the file (at most 8 reads) */
    while ((len=read(fd, buf, 512)) > 0) {
	retval += len;
    }
    close(fd);
    return(retval);
}
#endif /* XINU8 */
