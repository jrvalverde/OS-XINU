/* sgmp.h */

/* message types */

#define	SG_GET		1		/* get request	*/
#define	SG_GETRSP	2		/* get response	*/
#define	SG_TRAP		3		/* trap request	*/
#define	SG_SET		4		/* set request	*/

/* defines for error status value */

#define	SGMP_NOERR	0
#define	SGMP_TOOBIG	1
#define SGMP_NIXNAM	2
#define SGMP_BADVAL	3

struct	sgmp {
	int	sg_id;			/* request id		*/
	int	sg_errstat;		/* error status		*/
	int	sg_errindex;		/* error index		*/
	int	sg_data[1];		/* var op list		*/
};

/* recognized names, lengths and data areas */

struct sgmpvt {
	char	*sv_name;		/* variable name string	*/
	int	sv_numsize;		/* in octets		*/
	char	*sv_num;		/* numeric name		*/
	char	*sv_value;		/* local address	*/
	int	sv_valsize;		/* value size in octets	*/
	int	sv_type;
};

/* sv_types */

#define	STRING	0x1
#define	INTEGER	0x2

extern	struct	sgmpvt	sgmpvar[];

extern	char	vers[];
