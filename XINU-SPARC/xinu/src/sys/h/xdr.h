/* xdr.h */

/* sizes of various data types */
#define XDRINTLEN		4
#define XDRUNSIGNEDLEN		4
#define XDRFLOATLEN		4
#define XDRDOUBLELEN		8

/* supported data types */
#define XDRT_UINT	1
#define XDRT_ENUM	2
#define XDRT_BOOL	3
#define XDRT_INT	4
#define XDRT_FLOAT	5
#define XDRT_DOUBLE	6
#define XDRT_STRING	7
#define XDRT_FOPAQ	8
#define XDRT_VOPAQ	9

#define XDRT_NAMES {"", "UINT", "ENUM", "BOOL", "INT", "FLOAT", \
			"DOUBLE", "STRING", "FOPAQ", "VOPAQ"}
extern char *xdrt_names[];

/* error constants for XDR conversion routines */
#define XDR_NOSPACE		-2

/* conversion routines - host --> XDR */
char *xdrsetint();
char *xdrsetunsigned();
char *xdrsetfloat();
char *xdrsetdouble();
char *xdrsetstring();
char *xdrsetfopaque();
char *xdrsetvopaque();

/* conversion routines - XDR --> host */
char *xdrgetint();
char *xdrgetunsigned();
char *xdrgetfloat();
char *xdrgetdouble();
char *xdrgetstring();
char *xdrgetfopaque();
char *xdrgetvopaque();

/* round up to a 4 byte boundary */
#define ROUND_4BYTE(x) ((((unsigned) (x)) + 3) & 0xFFFFFFFC)
