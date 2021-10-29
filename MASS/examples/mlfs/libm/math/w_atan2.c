/*!
 * -------------------------------------------------------------------------------------------------
 * \file w_atan2.c
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

/* @(#)w_atan2.c 5.1 93/09/24 */
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
FUNCTION
        <<atan2>>, <<atan2f>>---arc tangent of y/x

INDEX
   atan2
INDEX
   atan2f

ANSI_SYNOPSIS
        #include <math.h>
        double atan2(double <[y]>,double <[x]>);
        float atan2f(float <[y]>,float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double atan2(<[y]>,<[x]>);
        double <[y]>;
        double <[x]>;

        float atan2f(<[y]>,<[x]>);
        float <[y]>;
        float <[x]>;

DESCRIPTION

<<atan2>> computes the inverse tangent (arc tangent) of <[y]>/<[x]>. 
<<atan2>> produces the correct result even for angles near 
@ifnottex
pi/2 or -pi/2 
@end ifnottex
@tex
$\pi/2$ or $-\pi/2$
@end tex
(that is, when <[x]> is near 0). 

<<atan2f>> is identical to <<atan2>>, save that it takes and returns
<<float>>. 

RETURNS
<<atan2>> and <<atan2f>> return a value in radians, in the range of 
@ifnottex
-pi to pi.
@end ifnottex
@tex
$-\pi$ to $\pi$.
@end tex

You can modify error handling for these functions using <<matherr>>.

PORTABILITY
<<atan2>> is ANSI C.  <<atan2f>> is an extension.


*/

/* 
 * wrapper atan2(y,x)
 */

#include "fdlibm.h"
#include <errno.h>

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double atan2(double y, double x)	/* wrapper atan2 */
#else
	double atan2(y,x)			/* wrapper atan2 */
	double y,x;
#endif
{
#ifdef MLFS_FPU_DAZ
	y *= __volatile_one;
	x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

	return __ieee754_atan2(y,x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
