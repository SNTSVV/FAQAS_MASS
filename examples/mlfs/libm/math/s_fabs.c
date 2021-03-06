/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_fabs.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for fabs.
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

/* @(#)s_fabs.c 5.1 93/09/24 */
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
FUNCTION
       <<fabs>>, <<fabsf>>---absolute value (magnitude)
INDEX
	fabs
INDEX
	fabsf

ANSI_SYNOPSIS
	#include <math.h>
       double fabs(double <[x]>);
       float fabsf(float <[x]>);

TRAD_SYNOPSIS
	#include <math.h>
       double fabs(<[x]>) 
       double <[x]>;

       float fabsf(<[x]>)
       float <[x]>;

DESCRIPTION
<<fabs>> and <<fabsf>> calculate 
@tex
$|x|$, 
@end tex
the absolute value (magnitude) of the argument <[x]>, by direct
manipulation of the bit representation of <[x]>.

RETURNS
The calculated value is returned.  No errors are detected.

PORTABILITY
<<fabs>> is ANSI.
<<fabsf>> is an extension.

*/

/*
 * fabs(x) returns the absolute value of x.
 */

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double fabs(double x)
#else
	double fabs(x)
	double x;
#endif
{
#ifdef MLFS_FPU_DAZ
	x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

	__uint32_t high;
	GET_HIGH_WORD(high,x);
	SET_HIGH_WORD(x,high&0x7fffffff);
        return x;
}

#endif /* _DOUBLE_IS_32BITS */
