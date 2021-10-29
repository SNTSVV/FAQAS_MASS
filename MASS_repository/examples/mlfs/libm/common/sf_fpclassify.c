/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_fpclassify.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for __fpclassifyf.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * __fpclassifyd is the single precision version of fpclassify. It returns a predefined constant
 * integer depending on the type of input. There are different constants for zero, subnormal,
 * normal, infinite, and NaN.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|------------------------------------------------------
 * 2017-06-07 | F. Schriever | Add FPU DAZ behaviour #ifdef.
 * 2017-08-18 | F. Schriever | Remove not needed comparison as it was always true.
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* Copyright (C) 2002,2007 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include "fdlibm.h"

int
__fpclassifyf (float x)
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

  __uint32_t w;

  GET_FLOAT_WORD(w,x);
  
  if (w == 0x00000000 || w == 0x80000000U)
    return FP_ZERO;
  else if ((w >= 0x00800000 && w <= 0x7f7fffff) ||
           (w >= 0x80800000U && w <= 0xff7fffffU))
    return FP_NORMAL;
  else if (w <= 0x007fffff ||
           (w >= 0x80000001U && w <= 0x807fffffU))
    return FP_SUBNORMAL;
  else if (w == 0x7f800000 || w == 0xff800000U)
    return FP_INFINITE;
  else
    return FP_NAN;
}

