/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_modf.h
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
 * This version is for single precision floating point values.
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

/* sf_modf.c -- float version of s_modf.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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

#include "fdlibm.h"
#include <assert.h>

static volatile float one = 1.0f;

#ifdef __STDC__
	float modff(float x, float *iptr)
#else
	float modff(x, iptr)
	float x,*iptr;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

	float _xi = 0;
	__int32_t i0,j0;
	__uint32_t i;
	assert(iptr != (void*)0);
	if(iptr == (void*)0) {
	    iptr = &_xi;
	}
	GET_FLOAT_WORD(i0,x);
	j0 = ((i0>>23)&0xff)-0x7f;	/* exponent of x */
	if(j0<23) {			/* integer part in x */
	    if(j0<0) {			/* |x|<1 */
	        SET_FLOAT_WORD(*iptr,i0&0x80000000U);	/* *iptr = +-0 */
		return x;
	    } else {
		i = (0x007fffff)>>j0;
		if((i0&i)==0) {			/* x is integral */
		    __uint32_t ix;
		    *iptr = x;
		    GET_FLOAT_WORD(ix,x);
		    SET_FLOAT_WORD(x,ix&0x80000000U);	/* return +-0 */
		    return x;
		} else {
		    SET_FLOAT_WORD(*iptr,i0&(~i));
		    return x - *iptr;
		}
	    }
	} else {			/* no fraction part */
	    __uint32_t ix;
	    *iptr = x*one;
	    GET_FLOAT_WORD(ix,x);
	    if (FLT_UWORD_IS_NAN(ix&0x7fffffffU)) { return x+x; } /* x is NaN, return NaN */
	    SET_FLOAT_WORD(x,ix&0x80000000U);	/* return +-0 */
	    return x;
	}
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double modf(double x, double *iptr)
#else
	double modf(x, iptr)
	double x,*iptr;
#endif
{
	return (double) modff((float) x, (float *) iptr);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
