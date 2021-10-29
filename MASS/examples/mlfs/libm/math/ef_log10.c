/*!
 * -------------------------------------------------------------------------------------------------
 * \file ef_log10.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for the log10f procedure.
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
 * 2017-08-17 | F. Schriever | Use __kernel_log, according to https://github.com/freebsd/freebsd/commit/763a6159cfdbe9ac1004795e4dc32e7718d32769
 * 2017-08-17 | F. Schriever | Improve coordination with __kernel_log, according to https://github.com/freebsd/freebsd/commit/2772ec758e6fb6c0bdaa3e30a268816e754de5a7
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
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

/*
 * Return the base 10 logarithm of x. See e_log10.c and kf_log.h for most comments.
 */

#include "fdlibm.h"
#include "kf_log.h"

#ifdef __STDC__
static const float
#else
static float
#endif
two25      =  3.3554432000e+07f, /* 0x4c000000 */
ivln10hi   =  4.3432617188e-01f, /* 0x3ede6000 */
ivln10lo   = -3.1689971365e-05f, /* 0xb804ead9 */
log10_2hi  =  3.0102920532e-01f, /* 0x3e9a2080 */
log10_2lo  =  7.9034151668e-07f; /* 0x355427db */

#ifdef __STDC__
static const float zero   =  0.0f;
#else
static float zero   =  0.0f;
#endif

#ifdef __STDC__
	float __ieee754_log10f(float x)
#else
	float __ieee754_log10f(x)
	float x;
#endif
{
	float f,hfsq,hi,lo,r,y;
	__int32_t i,k,hx;

	GET_FLOAT_WORD(hx,x);

        k=0;
        if (FLT_UWORD_IS_ZERO(hx&0x7fffffff))
            return -two25/zero;             /* log(+-0)=-inf */
        if (hx<0) return (x-x)/zero;        /* log(-#) = NaN */
	if (!FLT_UWORD_IS_FINITE(hx)) return x+x;
        if (FLT_UWORD_IS_SUBNORMAL(hx)) {
            k -= 25; x *= two25; /* subnormal number, scale up x */
	    GET_FLOAT_WORD(hx,x);
        }
    if (hx == 0x3f800000) return zero; /* log(1) = +0 */
	k += (hx>>23)-127;
    hx &= 0x007fffff;
    i = (hx+(0x4afb0d))&0x800000;
    SET_FLOAT_WORD(x,hx|(i^0x3f800000));    /* normalize x or x/2 */
    k += (i>>23);
    y = (float)k;
    f = x - (float)1.0;
    hfsq = (float)0.5*f*f;
    r = __kernel_logf(f);
    hi = f - hfsq;
    GET_FLOAT_WORD(hx,hi);
    SET_FLOAT_WORD(hi,hx&0xfffff000U);
    lo = (f - hi) - hfsq + r;
    return y*log10_2lo + (lo+hi)*ivln10lo + lo*ivln10hi + hi*ivln10hi + y*log10_2hi;
}
