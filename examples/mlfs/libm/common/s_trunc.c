/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_trunc.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for trunc.
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
<<trunc>>, <<truncf>>---round to integer, towards zero
INDEX
	trunc
INDEX
	truncf

ANSI_SYNOPSIS
	#include <math.h>
	double trunc(double <[x]>);
	float truncf(float <[x]>);

DESCRIPTION
	The <<trunc>> functions round their argument to the integer value, in
	floating format, nearest to but no larger in magnitude than the
	argument, regardless of the current rounding direction.  (While the
	"inexact" floating-point exception behavior is unspecified by the C
	standard, the <<trunc>> functions are written so that "inexact" is not
	raised if the result does not equal the argument, which behavior is as
	recommended by IEEE 754 for its related functions.)

RETURNS
<[x]> truncated to an integral value.

PORTABILITY
ANSI C, POSIX

*/

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double trunc(double x)
#else
	double trunc(x)
	double x;
#endif
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

  __int32_t sign;
  /* Most significant word, least significant word. */
  __int32_t msw;
  __uint32_t lsw;
  __int32_t exponent_less_1023;

  EXTRACT_WORDS(msw, lsw, x);

  /* Extract sign bit. */
  sign = msw & 0x80000000U;

  /* Extract exponent field. */
  exponent_less_1023 = ((msw & 0x7ff00000) >> 20) - 1023;

  if (exponent_less_1023 < 20)
    {
      /* All significant digits are in msw. */
      if (exponent_less_1023 < 0)
        {
          /* -1 < x < 1, so result is +0 or -0. */
          INSERT_WORDS(x, sign, 0);
        }
      else
        {
          /* All relevant fraction bits are in msw, so lsw of the result is 0. */
          INSERT_WORDS(x, sign | (msw & ~(0x000fffff >> exponent_less_1023)), 0);
        }
    }
  else if (exponent_less_1023 > 51)
    {
      if (exponent_less_1023 == 1024)
        {
          /* x is infinite, or not a number, so trigger an exception. */
          return x + x;
        }
      /* All bits in the fraction fields of the msw and lsw are needed in the result. */
    }
  else
    {
      /* All fraction bits in msw are relevant.  Truncate irrelevant
         bits from lsw. */
      INSERT_WORDS(x, msw, lsw & ~(0xffffffffU >> (exponent_less_1023 - 20)));
    }
  return x;
}

#endif /* _DOUBLE_IS_32BITS */
