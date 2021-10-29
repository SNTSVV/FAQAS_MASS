/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_fpclassify.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for __fpclassifyd.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * __fpclassifyd is the double precision version of fpclassify. It returns a predefined constant
 * integer depending on the type of input. There are different constants for zero, subnormal,
 * normal, infinite, and NaN.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|------------------------------------------------------
 * 2017-05-19 | F. Schriever | Remove not needed comparison of unsigned integer >= 0
 * 2017-06-07 | F. Schriever | Add FPU DAZ behaviour #ifdef.
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* Copyright (C) 2002, 2007 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include "fdlibm.h"

int
__fpclassifyd (double x)
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

  __uint32_t msw, lsw;

  EXTRACT_WORDS(msw,lsw,x);

  if ((msw == 0x00000000 && lsw == 0x00000000) ||
      (msw == 0x80000000U && lsw == 0x00000000))
    return FP_ZERO;
  else if ((msw >= 0x00100000 && msw <= 0x7fefffff) ||
           (msw >= 0x80100000U && msw <= 0xffefffffU))
    return FP_NORMAL;
  else if ((msw <= 0x000fffff) ||
           (msw >= 0x80000000U && msw <= 0x800fffffU))
    /* zero is already handled above */
    return FP_SUBNORMAL;
  else if ((msw == 0x7ff00000 && lsw == 0x00000000) ||
           (msw == 0xfff00000U && lsw == 0x00000000))
    return FP_INFINITE;
  else
    return FP_NAN;
}
