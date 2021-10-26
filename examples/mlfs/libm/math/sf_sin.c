/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_sin.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for sin.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * -
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|------------------------------
 * 2017-06-07 | F. Schriever | Add FPU DAZ behaviour #ifdef.
 * 2017-08-17 | F. Schriever | Move optimisation for tiny x from kernel to external procedure, according to https://github.com/freebsd/freebsd/commit/5931c79161136c38c59ced85bd9c919fb37c8710
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* sf_sin.c -- float version of s_sin.c.
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

#ifdef __STDC__
	float sinf(float x)
#else
	float sinf(x)
	float x;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

	float y[2],z=0.0f;
	__int32_t n,ix;

	GET_FLOAT_WORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffff;
    if(ix <= 0x3f490fd8) {
        if(ix<0x39800000)           /* if x < 2**-12 */
        if(((int)x)==0) return x;   /* generate inexact */
        return __kernel_sinf(x,z,0);
    }

    /* sin(Inf or NaN) is NaN */
	else if (!FLT_UWORD_IS_FINITE(ix)) return x-x;

    /* argument reduction needed */
	else {
	    n = __ieee754_rem_pio2f(x,y);
	    switch(n&3) {
		case 0: return  __kernel_sinf(y[0],y[1],1);
		case 1: return  __kernel_cosf(y[0],y[1]);
		case 2: return -__kernel_sinf(y[0],y[1],1);
		default:
			return -__kernel_cosf(y[0],y[1]);
	    }
	}
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double sin(double x)
#else
	double sin(x)
	double x;
#endif
{
	return (double) sinf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
