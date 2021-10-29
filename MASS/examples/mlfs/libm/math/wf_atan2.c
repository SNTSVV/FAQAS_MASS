/*!
 * -------------------------------------------------------------------------------------------------
 * \file wf_atan2.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for atan2.
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
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* wf_atan2.c -- float version of w_atan2.c.
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
 *
 */

/* 
 * wrapper atan2f(y,x)
 */

#include "fdlibm.h"
#include <errno.h>

#ifdef __STDC__
	float atan2f(float y, float x)		/* wrapper atan2f */
#else
	float atan2f(y,x)			/* wrapper atan2 */
	float y,x;
#endif
{
#ifdef MLFS_FPU_DAZ
	y *= __volatile_onef;
	x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

	return __ieee754_atan2f(y,x);
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double atan2(double y, double x)
#else
	double atan2(y,x)
	double y,x;
#endif
{
	return (double) atan2f((float) y, (float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
