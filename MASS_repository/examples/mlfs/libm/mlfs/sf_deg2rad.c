/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_deg2rad.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for conversion from degree to equivalent radians value.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \author GTD GmbH, Fabian Schriever
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * The function deg2radf shall return the radians value of an angle given in degrees, which is the
 * result of x/180*pi.
 * This version is for single precision floating point values.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|---------------
 * 2017-05-09 | F. Schriever | First creation
 * 
 * \copyright Copyright European Space Agency, 2017
 * \par License
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 */

#include "fdlibm.h"

static const float pio180    =  1.745329238e-02f; /* 0x3C8EFA35 */

float deg2radf(float x)
{
#ifdef MLFS_FPU_DAZ
    x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

    __int32_t ix;

    /* Float word of x. */
    GET_FLOAT_WORD(ix,x);

    /* Check for infinities and NaN. */
    ix &= 0x7fffffff;
    if (ix>=0x7f800000) return x+x;

    /* Return x * pi/180. */
    return x * pio180;
}

#ifdef _DOUBLE_IS_32BITS

double deg2rad(double x)
{
    return (double) deg2radf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
