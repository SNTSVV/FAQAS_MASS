/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_cos.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for cos.
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
 * 2017-08-17 | F. Schriever | Improve thresholds for tiny x, according to https://github.com/freebsd/freebsd/commit/b0f6e7189c4ef7b5cddb70b4839db87ad4e5d7e8
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* @(#)s_cos.c 5.1 93/09/24 */
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

/* cos(x)
 * Return cosine function of x.
 *
 * kernel function:
 *	__kernel_sin		... sine function on [-pi/4,pi/4]
 *	__kernel_cos		... cosine function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on 
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2 
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 T
 *	    1	       C	  -S		-1/T
 *	    2	      -S	  -C		 T
 *	    3	      -C	   S		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded 
 */

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double cos(double x)
#else
	double cos(x)
	double x;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

	double y[2],z=0.0;
	__int32_t n,ix;

    /* High word of x. */
	GET_HIGH_WORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffff;
    if(ix <= 0x3fe921fb) {
        if(ix<0x3e46a09e)           /* if x < 2**-27 * sqrt(2) */
        if(((int)x)==0) return 1.0; /* generate inexact */
        return __kernel_cos(x,z);
    }

    /* cos(Inf or NaN) is NaN */
	else if (ix>=0x7ff00000) return x-x;

    /* argument reduction needed */
	else {
	    n = __ieee754_rem_pio2(x,y);
	    switch(n&3) {
		case 0: return  __kernel_cos(y[0],y[1]);
		case 1: return -__kernel_sin(y[0],y[1],1);
		case 2: return -__kernel_cos(y[0],y[1]);
		default:
		        return  __kernel_sin(y[0],y[1],1);
	    }
	}
}

#endif /* _DOUBLE_IS_32BITS */
