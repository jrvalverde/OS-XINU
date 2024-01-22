/* doprnt.c - _doprnt, _prt10, _prt8, _prt16 */

#define	MAXSTR	80
#define	LOCAL	static

/*------------------------------------------------------------------------
 *  _doprnt --  format and write output using 'func' to write characters
 *------------------------------------------------------------------------
 */
_doprnt(fmt, args, func, farg)	/* adapted by S. Salisbury, Purdue U.	*/
	char	*fmt;		/* Format string for printf		*/
	int	*args;		/* Arguments to	printf			*/
	int	(*func)();	/* Function to put a character		*/
	int	farg;		/* Argument to func			*/
{
	int	c;
	int	i;
	int	f;		/* The format character	(comes after %)	*/
	char	*str;		/* Running pointer in string		*/
	char	string[20];	/* The string str points to this output	*/
				/*  from number conversion		*/
	int	length;		/* Length of string "str"		*/
	char	fill;		/* Fill	character (' ' or '0')		*/
	int	leftjust;	/* 0 = right-justified,	else left-just.	*/
	int	fmax,fmin;	/* Field specifications	% MIN .	MAX s	*/
	int	leading;	/* No. of leading/trailing fill	chars.	*/
	char	sign;		/* Set to '-' for negative decimals	*/
	char	digit1;		/* Offset to add to first numeric digit	*/

	for(;;) {
		/* Echo characters until '%' or end of fmt string */
		while( (c = *fmt++) != '%' ) {
			if( c == '\0' )
				return;
			(*func)(farg,c);
		}
		/* Echo "...%%..." as '%' */
		if( *fmt == '%' ) {
			(*func)(farg,*fmt++);
			continue;
		}
		/* Check for "%-..." == Left-justified output */
		if (leftjust = ((*fmt=='-') ? 1 : 0) )
			fmt++;
		/* Allow for zero-filled numeric outputs ("%0...") */
		fill = (*fmt=='0') ? *fmt++ : ' ';
		/* Allow for minimum field width specifier for %d,u,x,o,c,s*/
		/* Also allow %* for variable width (%0* as well)	*/
		fmin = 0;
		if( *fmt == '*' ) {
			fmin = *args++;
			++fmt;
		}
		else while( '0' <= *fmt && *fmt <= '9' ) {
			fmin = fmin * 10 + *fmt++ - '0';
		}
		/* Allow for maximum string width for %s */
		fmax = 0;
		if( *fmt == '.' ) {
			if( *(++fmt) == '*' ) {
				fmax = *args++;
				++fmt;
			}
		else while( '0' <= *fmt && *fmt <= '9' ) {
			fmax = fmax * 10 + *fmt++ - '0';
			}
		}
		if (*fmt == 'l') /* long */
			fmt++;
		str = string;
		if( (f= *fmt++) == '\0' ) {
			(*func)(farg,'%');
			return;
		}
		sign = '\0';	/* sign == '-' for negative decimal */

		switch( f ) {
		    case 'c' :
			string[0] = (char) *args;
			string[1] = '\0';
			fmax = 0;
			fill = ' ';
			break;

		    case 's' :
			str = (char *) *args;
			fill = ' ';
			break;

		    case 'D' :
		    case 'd' :
			if ( *args < 0 ) {
				sign = '-';
				*args = -*args;
				}
		    case 'U':
		    case 'u':
			_prt10(*args, str);
			fmax = 0;
			break;

		    case 'O' :
		    case 'o' :
			_prt8(*args, str);
			fmax = 0;
			break;

		    case 'X' :
		    case 'x' :
			_prt16(*args, str);
			fmax = 0;
			break;

		    default :
			(*func)(farg,f);
			break;
		}
		args++;
		for(length = 0; str[length] != '\0'; length++)
			;
		if ( fmin > MAXSTR || fmin < 0 )
			fmin = 0;
		if ( fmax > MAXSTR || fmax < 0 )
			fmax = 0;
		leading = 0;
		if ( fmax != 0 || fmin != 0 ) {
			if ( fmax != 0 )
				if ( length > fmax )
					length = fmax;
			if ( fmin != 0 )
				leading = fmin - length;
			if ( sign == '-' )
				--leading;
		}
		if( sign == '-' && fill == '0' )
			(*func)(farg,sign);
		if( leftjust == 0 )
			for( i = 0; i < leading; i++ )
				(*func)(farg,fill);
		if( sign == '-' && fill == ' ' )
			(*func)(farg,sign);
		for( i = 0 ; i < length ; i++ )
			(*func)(farg,str[i]);
		if ( leftjust != 0 )
			for( i = 0; i < leading; i++ )
				(*func)(farg,fill);
	}
}

LOCAL	_prt10(num,str)
	unsigned long	num;
	char	*str;
{
	int	i;
	char	c, temp[11];

	temp[0]	= '\0';
	for(i = 1; i <= 10; i++)  {
		temp[i] = num % 10 + '0';
		num /= 10;
	}
	for(i = 10; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prt8(num,str)
	unsigned long	num;
	char	*str;
{
	int	i;
	char	c, temp[12];

	temp[0]	= '\0';
	for(i = 1; i <= 11; i++)  {
		temp[i]	= (num & 07) + '0';
		num = num >> 3;
	}
	temp[11] &= '3';
	for(i = 11; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}

LOCAL	_prt16(num,str)
	unsigned long	num;
	char	*str;
{
	int	i;
	char	c, temp[9];

	temp[0] = '\0';
	for(i = 1; i <= 8; i++)  {
		temp[i] = "0123456789abcdef"[num & 0x0f];
		num = num >> 4;
	}
	for(i = 8; temp[i] == '0'; i--);
	if( i == 0 )
		i++;
	while( i >= 0 )
		*str++ = temp[i--];
}
