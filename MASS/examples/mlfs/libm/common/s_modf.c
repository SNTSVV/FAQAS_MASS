/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_modf.h
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for modf.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * Splits input floating point argument into an integral and a fractional part.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|------------------------------
 * 2017-06-07 | F. Schriever | Add NULL-pointer handling.
 * 2017-06-07 | F. Schriever | Add FPU DAZ behaviour #ifdef.
 * 2017-08-18 | F. Schriever | Make variable one volatile to enforce correct behaviour with regards to exceptions (otherwise the calculation may no longer exist after compilation due to optimization).
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* @(#)s_modf.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
FUNCTION
       <<modf>>, <<modff>>---split fractional and integer parts

INDEX
	modf
INDEX
	modff

ANSI_SYNOPSIS
	#include <math.h>
	double modf(double <[val]>, double *<[ipart]>);
        float modff(float <[val]>, float *<[ipart]>);

TRAD_SYNOPSIS
	#include <math.h>
	double modf(<[val]>, <[ipart]>)
        double <[val]>;
        double *<[ipart]>;

	float modff(<[val]>, <[ipart]>)
	float <[val]>;
        float *<[ipart]>;

DESCRIPTION
	<<modf>> splits the double <[val]> apart into an integer part
	and a fractional part, returning the fractional part and
	storing the integer part in <<*<[ipart]>>>.  No rounding
	whatsoever is done; the sum of the integer and fractional
	parts is guaranteed to be exactly  equal to <[val]>.   That
	is, if <[realpart]> = modf(<[val]>, &<[intpart]>); then
	`<<<[realpart]>+<[intpart]>>>' is the same as <[val]>.
	<<modff>> is identical, save that it takes and returns
	<<float>> rather than <<double>> values.

RETURNS
	The fractional part is returned.  Each result has the same
	sign as the supplied argument <[val]>.

PORTABILITY
	<<modf>> is ANSI C. <<modff>> is an extension.

QUICKREF
	modf  ansi pure
	modff - pure

*/

/*
 * modf(double x, double *iptr)
 * return fraction part of x, and return x's integral part in *iptr.
 * Method:
 *	Bit twiddling.
 *
 * Exception:
 *	No exception.
 */

#include "fdlibm.h"
#include <assert.h>

#ifndef _DOUBLE_IS_32BITS

static volatile double one = 1.0;

#ifdef __STDC__
	double modf(double x, double *iptr)
#else
	double modf(x, iptr)
	double x,*iptr;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

	double _xi = 0;
	__int32_t i0,i1,j0;
	__uint32_t i;
	assert(iptr != (void*)0);
	if(iptr == (void*)0) {
	    iptr = &_xi;
	}
	EXTRACT_WORDS(i0,i1,x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;	/* exponent of x */
	if(j0<20) {			/* integer part in high x */
	    if(j0<0) {			/* |x|<1 */
	        INSERT_WORDS(*iptr,i0&0x80000000U,0U);	/* *iptr = +-0 */
		return x;
	    } else {
		i = (0x000fffff)>>j0;
		if(((i0&i)|i1)==0) {		/* x is integral */
		    __uint32_t high;
		    *iptr = x;
		    GET_HIGH_WORD(high,x);
		    INSERT_WORDS(x,high&0x80000000U,0U);	/* return +-0 */
		    return x;
		} else {
		    INSERT_WORDS(*iptr,i0&(~i),0);
		    return x - *iptr;
		}
	    }
	} else if (j0>51) {		/* no fraction part */
	    __uint32_t high, low;
	    *iptr = x*one;
	    EXTRACT_WORDS(high, low, x);
	    if ((high&0x7ff00000U)==0x7ff00000U && ((high&0x000fffffU)|low)!= 0) { return x+x; } /* x is NaN, return NaN */
	    INSERT_WORDS(x,high&0x80000000U,0U);	/* return +-0 */
	    return x;
	} else {			/* fraction part in low x */
	    i = ((__uint32_t)(0xffffffffU))>>(j0-20);
	    if((i1&i)==0) { 		/* x is integral */
	        __uint32_t high;
		*iptr = x;
		GET_HIGH_WORD(high,x);
		INSERT_WORDS(x,high&0x80000000U,0U);	/* return +-0 */
		return x;
	    } else {
	        INSERT_WORDS(*iptr,i0,i1&(~i));
		return x - *iptr;
	    }
	}
}

#endif /* _DOUBLE_IS_32BITS */
