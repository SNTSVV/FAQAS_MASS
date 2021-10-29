/*!
 * -------------------------------------------------------------------------------------------------
 * \file wf_sqrt.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for sqrt.
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

/* wf_sqrt.c -- float version of w_sqrt.c.
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
 * wrapper sqrtf(x)
 */

#include "fdlibm.h"
#include <errno.h>

#ifdef __STDC__
	float sqrtf(float x)		/* wrapper sqrtf */
#else
	float sqrtf(x)			/* wrapper sqrtf */
	float x;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

#ifdef _IEEE_LIBM
	return __ieee754_sqrtf(x);
#else
	float z;
	struct exception exc;
	z = __ieee754_sqrtf(x);
	if(_LIB_VERSION == _IEEE_ || isnan(x)) return z;
	if(x<(float)0.0) {
            /* sqrtf(negative) */
            exc.type = DOMAIN;
            exc.name = "sqrtf";
	    exc.err = 0;
	    exc.arg1 = exc.arg2 = (double)x;
            if (_LIB_VERSION == _SVID_)
              exc.retval = 0.0;
            else
              exc.retval = 0.0/0.0;
            if (_LIB_VERSION == _POSIX_) 
              errno = EDOM;
            else if (!matherr(&exc)) {
              errno = EDOM;
            }
            if (exc.err != 0)
	      errno = exc.err;
	    return (float)exc.retval; 
	} else
	    return z;
#endif
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double sqrt(double x)
#else
	double sqrt(x)
	double x;
#endif
{
	return (double) sqrtf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
