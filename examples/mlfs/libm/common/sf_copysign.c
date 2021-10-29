/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_copysign.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for copysign.
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

/* sf_copysign.c -- float version of s_copysign.c.
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

/*
 * copysignf(float x, float y)
 * copysignf(x,y) returns a value with the magnitude of x and
 * with the sign bit of y.
 */

#include "fdlibm.h"

#ifdef __STDC__
	float copysignf(float x, float y)
#else
	float copysignf(x,y)
	float x,y;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_onef;
	y *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

	__uint32_t ix,iy;
	GET_FLOAT_WORD(ix,x);
	GET_FLOAT_WORD(iy,y);
	SET_FLOAT_WORD(x,(ix&0x7fffffffU)|(iy&0x80000000U));
        return x;
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double copysign(double x, double y)
#else
	double copysign(x,y)
	double x,y;
#endif
{
	return (double) copysignf((float) x, (float) y);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
