/* PDP-11 Assembler for the VAX Pass 1 */

#include "vars2.h"
#include "stypes.h"

int *aptr;

opline(op) {
	char optype, swapf;
	short opval, op1;
	int temp, rlimit, *aptr1;

	if((unsigned)op < 0200) {
		if(op == 05) {
			char *ptr;

			line = 1;
			for(ptr = argb; (short)(temp = getshort()) >= 0; ) {
				*ptr++ = temp;
				if(ptr == &argb[20]) ptr--;
			}
			*ptr++ = '\n';
			*ptr++ = 0;
			return(op);
		}
		else if(op == '<') goto ascii;
		else goto xpr;
	}
	optype = ((struct symbol *)op)->s_type;
	if(optype == TREG || optype == TETEXT || optype == TEDATA ||
	   optype <= TBSS || optype > TJCOND )
		goto xpr;
	opval = ((struct symbol *)op)->s_value;
	op = readop();
	swapf = 0;
	aptr = adrbuf;
	rlimit = -1;
	switch(optype) {
	    case TMUL:
		swapf++;
		rlimit = 01000;
		goto dble;

	    case TFLREG:
		swapf++;

	    case TFLDST:
		rlimit = 0400;

	    case TDBLOP:
	      dble:
		op = address(op, 0);
	      twoop:
		op1 = eval;
		op = address(readop(), 0);
	      single:
		if(swapf) {
			temp = op1;
			op1 = eval;
			eval = temp;
		}
		op1 <<= 6;
		if(op1 > (unsigned)rlimit) error('x');
                outw(0, opval | op1 | eval);    /* ins | field1 | field2 */
		for(aptr1 = adrbuf; aptr1 < aptr; ) {
			opval = *aptr1++;
			op1 = *aptr1++;
			xsymbol = (struct symbol *)(*aptr1++);
			outw(op1, opval);
		}
		return(op);

	    case TSINGLE:
		op = address(op, 0);
		goto single;

	    case TMOVF:
		rlimit = 0400;
		op = address(op, 0);
		if((unsigned)eval < 04) opval = 0174000;
		else swapf++;
		goto twoop;

	    case TCBYTE:
		for(;;op = readop()) {
			op = expres(op);
			outb(etype, eval);
			if(op != ',') break;
		}
		return(op);

	    case TCSTR:
	ascii:
		while(((temp = getshort()) & 0100000) == 0)
			outb(1, temp & 0377);
		return(getshort());

	    case TCEVEN:
		if(DOT & 1)
			if(DOTREL != 04) outb(0, 0);
			else DOT++;
		return(op);

	    case TCIF:
		op = expres(op);

	    case TCENDIF:
		return(op);

	    case TCGLOBL:
		for(;;op = readop()) {
			if((unsigned)op < 0200) return(op);
			((struct symbol *)op)->s_type |= TGLOBAL;
			if((op = readop()) != ',') return(op);
		}
		/* NOTREACHED */

	    case TREG: case TETEXT: case TEDATA:
	xpr:
		op = expres(op);
		outw(etype, eval);
		return(op);

	    case TCTEXT: case TCDATA: case TCBSS:
		DOT = (DOT + 1) & ~1;
		savdot[DOTREL-2] = DOT;
		if(passno) {
			flush(&txtp);
			flush(&relp);
			tseekp = &dataseeks[optype-TCTEXT];
			oset(&txtp, *tseekp);
			rseekp = &relseeks[optype-TCTEXT];
			oset(&relp, *rseekp);
		}
		DOT = savdot[optype-TCTEXT];
		DOTREL = optype - TCTEXT + 2;
		return(op);

	    case TCCOMM:
		if((unsigned)op >= 0200) {
			temp = op;
			op = readop();
			op = expres(readop());
			if( (((struct symbol *)temp)->s_type & 037) != 0)
				((struct symbol *)temp)->s_type |= TGLOBAL;
			((struct symbol *)temp)->s_value = eval;
		}
		return(op);

	    case TJBR: case TJCOND:
		op = expres(op);
		if(!passno) {
			if((eval = setbr(eval)) != 0 && opval != BR) eval += 2;
			DOT += eval + 2;
			return(op);
		}
		if(!getbr()) goto dobranch;
		if(opval != BR) outw(1, opval ^ 0402);
		outw(1, JMP+037);
		outw(etype, eval);
		return(op);

	    case TSOB:
		op = expres(op);
		checkreg();
		opval |= eval << 6;
		op = expres(readop());
		if(!passno) goto brpass0;
		eval = DOT - eval;
		if(eval < -2 || eval > 0175) goto berror;
		eval += 4;
		goto brcase;

	    case TBR:
		op = expres(op);
		if(passno) {
		    dobranch:
			eval -= DOT;
			if(eval < -254 || eval > 256) {
			    berror:
				error('b');
				eval = 0;
			} else {
			    brcase:
				if(eval & 1 || etype != DOTREL) goto berror;
				eval = ((eval >> 1) - 1) & 0377;
			}
		}
	      brpass0:
		outw(0, eval | opval);
		return(op);

	    case TJSR:
		op = expres(op);
		checkreg();
		goto twoop;

	    case TRTS:
		op = expres(op);
		checkreg();
		outw(etype, eval | opval);
		return(op);

	    case TSYS:
		op = expres(op);
		if((unsigned)eval >= 256. || etype > TABS) error('a');
		outw(etype, eval | opval);
		return(op);

	}
	/* NOTREACHED */
}

address(op, value) {
	switch(op) {
	    case '(':
		return(alp(value));

	    case '-':
		return(amin(value));

	    case '$':
		return(adoll(value));

	    case '*':
		return(astar(value));

	    default:
		return(getx(op, value));
	}
}

getx(op, value) {
	op = expres(op);
	if(op == '(') {
		*aptr++ = eval;
		*aptr++ = etype;
		*aptr++ = (int)xsymbol;
		op = expres(readop());
		checkreg();
		op = checkrp(op);
		eval |= value | 060;
		return(op);
	}
	if(etype == 024) {
		checkreg();
		eval |= value;
		return(op);
	}
	eval -= DOT + 4;
	if(aptr != adrbuf) eval -= 2;
	*aptr++ = eval;
	*aptr++ = etype | 0100000;
	*aptr++ = (int)xsymbol;
	eval = 067 | value;
	return(op);
}

alp(value) {
	int op;

	op = expres(readop());
	op = checkrp(op);
	checkreg();
	if(op == '+') {
		op = readop();
		eval |= value | 020;
	} else if(value == 0) eval |= 010;
	else {
		eval |= 070;
		*aptr++ = 0;
		*aptr++ = 0;
		*aptr++ = (int)xsymbol;
	}
	return(op);
}

amin(value) {
	int op;

	if( (op = readop()) != '(') {
		savop = op;
		op = '-';
		return(getx(op, value));
	}
	op = expres(readop());
	op = checkrp(op);
	checkreg();
	eval |= value | TGLOBAL;
	return(op);
}

adoll(value) {
	int op;

	op = expres(readop());
	*aptr++ = eval;
	*aptr++ = etype;
	*aptr++ = (int)xsymbol;
	eval = value | 027;
	return(op);
}

astar(value) {
	if(value != 0) error('*');
	return(address(readop(), 010));
}

checkreg() {
	if(eval <= 7 && (etype <= TABS || etype >= TFLDST)) return;
	error('a');
	etype = 0;
	eval = 0;
}

checkrp(op) {
	if(op != ')') {
		error(')');
		return(op);
	}
	return(readop());
}

setbr(loc) {
	register int pos;
	if((pos = brtabp) >= BRLEN) return(2);
	brtabp++;
	if((loc -= DOT) > 0) loc -= brdelt;
	if(loc >= -254 && loc <= 256) return(0);
	brtab[pos/8] |= 1 << (pos % 8);
	return(2);
}

getbr() {
	int temp;

	if(brtabp >= BRLEN) return(1);
	temp = brtab[brtabp/8] & (1 << (brtabp % 8));
	brtabp++;
	return(temp);
}
