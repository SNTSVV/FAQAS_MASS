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
 * The function rem2pif shall return the value of the argument x n-times divided by the value 2pi
 * such that: +0 <= x - n * 2pi <= 2pi
 * The implementation makes use of the already existing __ieee754_rem_pio2f function, that reduces
 * the given angle to the range [-pi/4,pi/4]. This value is then increased by adding multiples of
 * pi/2, depending on where the angle lies within the unit circle.
 * This version is for single precision floating point values.
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

static const float pio2_hi  =  1.570796370e+0f; /* 0x3FC90FDB */
static const float pio2_lo  = -4.371139000e-8f; /* 0xB33BBD2E */
static const float pi3o2_hi =  4.712388980e+0f; /* 0x4096CBE3 */
static const float pi3o2_lo = -1.192488045e-8f; /* 0xB24CDE2E */

float rem2pif(float x)
{
#ifdef MLFS_FPU_DAZ
    x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

    __int32_t ix,n;
    float y[2];

    GET_FLOAT_WORD(ix,x);

    /* Check for infinities and NaN. */
    if ((ix&0x7fffffff)>=0x7f800000) return x-x;

    /* Check for +-0. */
    if (0==(ix&0x7fffffff)) return x;

    /* Check if already in range. */
    if(ix < 0x40C90FDB && ix > 0) return x;

    /* Use pre-existing range reduction function, which reduces to [-pi/4, pi/4]. */
    n = __ieee754_rem_pio2f(x, y);

    /* Add multiples of pi/2 depending on which quadrant of the unit circle the angle lies in. */
    switch(n&3)
    {
        case 0:
            if ((y[0] + y[1]) < 0.0f)
            {
                y[0] += 4.0f*pio2_hi;
                y[1] += 4.0f*pio2_lo;
            }
            break;
        case 1:
            y[0] += pio2_hi;
            y[1] += pio2_lo;
            break;
        case 2:
            y[0] += 2.0f*pio2_hi;
            y[1] += 2.0f*pio2_lo;
            break;
        default:
            y[0] += pi3o2_hi;
            y[1] += pi3o2_lo;
            break;
    }
    return y[0] + y[1];
}

#ifdef _DOUBLE_IS_32BITS

double rem2pi(double x)
{
    return (double) rem2pif((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
