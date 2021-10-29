/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_rem2pi.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for range reduction to the interval [0,2pi].
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \author GTD GmbH, Fabian Schriever
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * The function rem2pi shall return the value of the argument x n-times divided by the value 2pi
 * such that: +0 <= x - n * 2pi <= 2pi
 * The implementation makes use of the already existing __ieee754_rem_pio2 function, that reduces
 * the given angle to the range [-pi/4,pi/4]. This value is then increased by adding multiples of
 * pi/2, depending on where the angle lies within the unit circle.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|---------------
 * 2017-05-15 | F. Schriever | First creation
 * 
 * \copyright Copyright European Space Agency, 2017
 * \par License
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 */

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

static const double pio2_hi  =  1.57079632679489655800e+00; /* 0x3FF921FB, 0x54442D18 */
static const double pio2_lo  =  6.12323399573676603587e-17; /* 0x3C91A626, 0x33145C07 */
static const double pi3o2_hi =  4.71238898038468985769e+00; /* 0x4012D97C, 0x7F3321D2 */
static const double pi3o2_lo =  1.83697019872102976584e-16; /* 0x3CAA7939, 0x4C9E8A0A */

double rem2pi(double x)
{
#ifdef MLFS_FPU_DAZ
    x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

    __int32_t hx,lx,n;
    double y[2];

    EXTRACT_WORDS(hx,lx,x);

    /* Check for infinities and NaN. */
    if ((hx&0x7fffffff)>=0x7ff00000) return x-x;

    /* Check for +-0. */
    if (0==((hx&0x7fffffff)|lx)) return x;

    /* Check if already in range. */
    if(hx < 0x401921fb && hx >= 0) return x;

    /* Use pre-existing range reduction function, which reduces to [-pi/4, pi/4]. */
    n = __ieee754_rem_pio2(x, y);

    /* Add multiples of pi/2 depending on which quadrant of the unit circle the angle lies in. */
    switch(n&3)
    {
        case 0:
            if ((y[0] + y[1]) < 0.0)
            {
                y[0] += 4.0*pio2_hi;
                y[1] += 4.0*pio2_lo;
            }
            break;
        case 1:
            y[0] += pio2_hi;
            y[1] += pio2_lo;
            break;
        case 2:
            y[0] += 2.0*pio2_hi;
            y[1] += 2.0*pio2_lo;
            break;
        default:
            y[0] += pi3o2_hi;
            y[1] += pi3o2_lo;
            break;
    }
    return y[0] + y[1];
}

#endif /* _DOUBLE_IS_32BITS */
