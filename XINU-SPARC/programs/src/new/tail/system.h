/* system-dependent definitions for fileutils programs.
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

/* Include sys/types.h before this file.  */

#include <sys/stat.h>
#include <sys/param.h>

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

#ifdef USG
#include <sys/sysmacros.h>
#include <fcntl.h>

char *getcwd ();
#define index strchr
#define rindex strrchr
#define bcopy(from, to, len) memcpy ((to), (from), (len))
#define getwd(buf) getcwd ((buf), MAXPATHLEN + 1)
#define L_SET 0
#define L_INCR 1
#define L_XTND 2
#define F_OK 0
#define W_OK 2
#define R_OK 4
#include <string.h>
#else
char *getwd ();
#include <strings.h>
#endif

#ifdef __GNUC__
#define alloca __builtin_alloca
#else
#ifdef sparc
#include <alloca.h>
#else
char *alloca ();
#endif
#endif

#ifdef USG
#define ST_BLKSIZE(statbuf) BLKSIZE
#else
#define ST_BLKSIZE(statbuf) ((statbuf).st_blksize)
#endif

#ifdef USG
#include <sys/times.h>
#else
#include <sys/time.h>
#endif

#ifdef USGr3
# include <dirent.h>
# define direct dirent
# define NLENGTH(direct) (strlen((direct)->d_name))
# ifndef DIRSIZ			/* Defined in param.h sometimes. */
#  define DIRSIZ(dp) DIRBUF
# endif
#else
# define NLENGTH(direct) ((direct)->d_namlen)
# ifdef USG
#  ifdef SYSNDIR
#   include <sys/ndir.h>
#  else
#   include <ndir.h>
#  endif
# else /* must be BSD */
#  include <sys/dir.h>
#  include <sys/file.h>
# endif
#endif

/* Extract the number of blocks from a `struct stat *'. */
#ifdef USG
#define ST_NBLOCKS(stat) (((stat).st_size + BLKSIZE - 1) / BLKSIZE)
#else
#define ST_NBLOCKS(stat) ((stat).st_blocks)
#endif

#ifndef S_IFLNK
#define lstat stat
#endif
