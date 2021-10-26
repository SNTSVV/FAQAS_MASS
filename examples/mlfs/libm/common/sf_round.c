/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_round.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for round.
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

#include "fdlibm.h"

#ifdef __STDC__
	float roundf(float x)
#else
	float roundf(x)
	float x;
#endif
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

  __uint32_t w;
  /* Most significant word, least significant word. */
  __int32_t exponent_less_127;

  GET_FLOAT_WORD(w, x);

  /* Extract exponent field. */
  exponent_less_127 = (__int32_t)((w & 0x7f800000) >> 23) - 127;

  if (exponent_less_127 < 23)
    {
      if (exponent_less_127 < 0)
        {
          w &= 0x80000000U;
          if (exponent_less_127 == -1)
            /* Result is +1.0 or -1.0. */
            w |= ((__uint32_t)127 << 23);
        }
      else
        {
          __uint32_t exponent_mask = 0x007fffff >> exponent_less_127;
          if ((w & exponent_mask) == 0)
            /* x has an integral value. */
            return x;

          w += 0x00400000 >> exponent_less_127;
          w &= ~exponent_mask;
        }
    }
  else
    {
      if (exponent_less_127 == 128)
        /* x is NaN or infinite. */
        return x + x;
      else
        return x;
    }
  SET_FLOAT_WORD(x, w);
  return x;
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double round(double x)
#else
	double round(x)
	double x;
#endif
{
	return (double) roundf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
