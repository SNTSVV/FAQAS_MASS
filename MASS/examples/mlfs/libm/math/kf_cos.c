/*!
 * -------------------------------------------------------------------------------------------------
 * \file kf_cos.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for the __kernel_cosf procedure.
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
 * 2017-08-17 | F. Schriever | Use better algorithm, according to https://github.com/freebsd/freebsd/commit/96c89ee304b32108340680ce85ac644a0a7d9b85
 * 2017-08-17 | F. Schriever | Use optimised polynomial, according to https://github.com/freebsd/freebsd/commit/96c89ee304b32108340680ce85ac644a0a7d9b85
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* kf_cos.c -- float version of k_cos.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 * Debugged and optimized by Bruce D. Evans.
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

/* Range of maximum relative error in polynomial: ~[-1.15e-10, 1.169e-10]. */
#ifdef __STDC__
static const float 
#else
static float 
#endif
one =  1.0000000000e+00f, /* 0x3f800000 */
C1  =  0xaaaaa5.0p-28f, /*  0.04166664555668830871582031250,    0x3D2AAAA5 */
C2  = -0xb60615.0p-33f, /* -0.001388731063343584537506103516,   0xBAB60615 */
C3  =  0xccf47d.0p-39f; /*  0.00002443254288664320483803749084, 0x37CCF47C */

#ifdef __STDC__
	float __kernel_cosf(float x, float y)
#else
	float __kernel_cosf(x, y)
	float x,y;
#endif
{
	float hz,z,r,w;

	z  = x*x;
	r  = z*(C1+z*(C2+z*C3));

	hz = (float)0.5*z;
	w  = one-hz;
	return w + (((one-w)-hz) + (z*r-x*y));
}
