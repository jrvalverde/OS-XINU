#ifndef _TYPES_
#define _TYPES_   /* so Unix won't include its types. */
#endif    

/*
 * Basic system types.
 */

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;		/* System V compatibility */
typedef	unsigned int	uint;		/* System V compatibility */

typedef	struct	_quad { long val[2]; } quad;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	u_long	ino_t;
typedef	long	swblk_t;

#ifndef _SIZE_T_
#define _SIZE_T_
typedef unsigned int    size_t;
#endif  /* _SIZE_T_ */

#ifndef _TIME_T_
#define _TIME_T_
typedef long time_t;
#endif /* _TIME_T_ */

#ifndef _CLOCK_T_
#define _CLOCK_T_
typedef long clock_t;
#endif /* _CLOCK_T_ */

typedef	short	dev_t;
typedef	long	off_t;
typedef u_short mode_t;                 /* POSIX compliance    */
typedef short   nlink_t;                /* POSIX compliance    */
typedef	u_short	uid_t;
typedef	u_short	gid_t;
typedef	long	key_t;
