/*!
 * -------------------------------------------------------------------------------------------------
 * \file kf_sin.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for the __kernel_sinf procedure.
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
 * 2017-08-17 | F. Schriever | Move optimisation for tiny x from kernel to external procedure, according to https://github.com/freebsd/freebsd/commit/5931c79161136c38c59ced85bd9c919fb37c8710
 * 2017-08-17 | F. Schriever | Use optimised polynomial, according to https://github.com/freebsd/freebsd/commit/96c89ee304b32108340680ce85ac644a0a7d9b85
 * 2017-08-17 | F. Schriever | Tweak polynomial, according to https://github.com/freebsd/freebsd/commit/fae8bfd4c4dc2e6c35f726257e68b291f7b9d358
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* kf_sin.c -- float version of k_sin.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 * Optimized by Bruce D. Evans.
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

/* |sin(x)/x - s(x)| < 2**-32.5 (~[-1.57e-10, 1.572e-10]). */
#ifdef __STDC__
static const float 
#else
static float 
#endif
half =  5.0000000000e-01f,/* 0x3f000000 */
S1  = -0xaaaaab.0p-26f, /* -0.16666667163,      0xBE2AAAAB */
S2  =  0x8888bb.0p-30f, /*  0.0083333803341,    0x3C0888BB */
S3  = -0xd02de1.0p-36f, /* -0.00019853517006,   0xB9502DE1 */
S4  =  0xbe6dbe.0p-42f; /*  0.0000028376084629, 0x363E6DBE */

#ifdef __STDC__
	float __kernel_sinf(float x, float y, __int32_t iy)
#else
	float __kernel_sinf(x, y, iy)
	float x,y; __int32_t iy;		/* iy=0 if y is zero */
#endif
{
	float z,r,v;

	z	=  x*x;
	v	=  z*x;
	r	=  S2+z*(S3+z*S4);
	if(iy==0) return x+v*(S1+z*r);
	else      return x-((z*(half*y-v*r)-y)-v*S1);
}
