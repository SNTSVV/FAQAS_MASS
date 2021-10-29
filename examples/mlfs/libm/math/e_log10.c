/*!
 * -------------------------------------------------------------------------------------------------
 * \file e_log10.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for the log10 procedure.
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
 * -----------|--------------|----------------------------------------------------------
 * 2017-08-17 | F. Schriever | Use __kernel_log, according to https://github.com/freebsd/freebsd/commit/c8691f6e6a0e57ecce0dc94678561826833214f4
 * 2017-08-17 | F. Schriever | Improve coordination with __kernel_log, according to https://github.com/freebsd/freebsd/commit/2772ec758e6fb6c0bdaa3e30a268816e754de5a7
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* @(#)e_log10.c 5.1 93/09/24 */
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

/* __ieee754_log10(x)
 * Return the base 10 logarithm of x. See e_log.c and k_log.h for most comments.
 *
 *    log10(x) = (f - 0.5*f*f + __kernel_log(f)) / ln10 + k * log10(2)
 * in not-quite-routine extra precision.
 */

#include "fdlibm.h"
#include "k_log.h"

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
static const double
#else
static double
#endif
two54      =  1.80143985094819840000e+16, /* 0x43500000, 0x00000000 */
ivln10hi   =  4.34294481878168880939e-01, /* 0x3fdbcb7b, 0x15200000 */
ivln10lo   =  2.50829467116452752298e-11, /* 0x3dbb9438, 0xca9aadd5 */
log10_2hi  =  3.01029995663611771306e-01, /* 0x3FD34413, 0x509F6000 */
log10_2lo  =  3.69423907715893078616e-13; /* 0x3D59FEF3, 0x11F12B36 */

#ifdef __STDC__
static const double zero   =  0.0;
#else
static double zero   =  0.0;
#endif

#ifdef __STDC__
	double __ieee754_log10(double x)
#else
	double __ieee754_log10(x)
	double x;
#endif
{
	double f,hfsq,hi,lo,r,val_hi,val_lo,w,y,y2;
	__int32_t i,k,hx;
	__uint32_t lx;

	EXTRACT_WORDS(hx,lx,x);

        k=0;
        if (hx < 0x00100000) {                  /* x < 2**-1022  */
            if (((hx&0x7fffffff)|lx)==0)
                return -two54/zero;             /* log(+-0)=-inf */
            if (hx<0) return (x-x)/zero;        /* log(-#) = NaN */
            k -= 54; x *= two54; /* subnormal number, scale up x */
	    GET_HIGH_WORD(hx,x);
        }
	if (hx >= 0x7ff00000) return x+x;
	if (hx == 0x3ff00000 && lx == 0) return zero; /* log(1) = +0 */
	k += (hx>>20)-1023;
    hx &= 0x000fffff;
    i = (hx+0x95f64)&0x100000;
    SET_HIGH_WORD(x,hx|(i^0x3ff00000)); /* normalize x or x/2 */
    k += (i>>20);
    y = (double)k;
    f = x - 1.0;
    hfsq = 0.5*f*f;
    r = __kernel_log(f);

    hi = f - hfsq;
    SET_LOW_WORD(hi,0);
    lo = (f - hi) - hfsq + r;
    val_hi = hi*ivln10hi;
    y2 = y*log10_2hi;
    val_lo = y*log10_2lo + (lo+hi)*ivln10lo + lo*ivln10hi;

    /*
     * Extra precision in for adding y*log10_2hi is not strictly needed
     * since there is no very large cancellation near x = sqrt(2) or
     * x = 1/sqrt(2), but we do it anyway since it costs little on CPUs
     * with some parallelism and it reduces the error for many args.
     */
    w = y2 + val_hi;
    val_lo += (y2 - w) + val_hi;
    val_hi = w;

    return val_lo + val_hi;
}

#endif /* defined(_DOUBLE_IS_32BITS) */
