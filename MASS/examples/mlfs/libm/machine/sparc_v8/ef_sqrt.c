/*!
 * -------------------------------------------------------------------------------------------------
 * \file ef_sqrt.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Replacement file for math/ef_sqrt.c. Switches to hardware sqrt on sparc v8 machines.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \author GTD GmbH, Fabian Schriever
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * Uses the hardware sqrt function of sparc v8 machines to return the square root of the input
 * argument. This file will used as a replacement for math/ef_sqrt.c when the hardware sqrt is
 * enabled at compilation and the machine is set to sparc_v8.
 * This version is for single precision floating point values.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|---------------
 * 2017-05-23 | F. Schriever | First creation
 * 
 * \copyright Copyright European Space Agency, 2017
 * \par License
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 */

#include "fdlibm.h"

float __ieee754_sqrtf(float x)
{
    float root = 0.0f;

    asm volatile ("fsqrts %[x], %[root]"
                  : [root] "=f" (root)
                  : [x] "f" (x));

    return root;
}
