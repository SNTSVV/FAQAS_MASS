/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_trunc.c
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
 * -----------|--------------|----------------------------------------------------------------------
 * 2017-06-06 | F. Schriever | Fix branch (else ... if), was 'if(exponent_less_127 == 255)' which made it possible to return a signaling NaN.
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
	float truncf(float x)
#else
	float truncf(x)
	float x;
#endif
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

  __int32_t sign, w, exponent_less_127;

  GET_FLOAT_WORD(w,x);

  /* Extract sign bit. */
  sign = w & 0x80000000U;

  /* Extract exponent field. */
  exponent_less_127 = ((w & 0x7f800000) >> 23) - 127;

  if (exponent_less_127 < 23)
    {
      if (exponent_less_127 < 0)
        {
          /* -1 < x < 1, so result is +0 or -0. */
          SET_FLOAT_WORD(x, sign);
        }
      else
        {
          SET_FLOAT_WORD(x, sign | (w & ~(0x007fffff >> exponent_less_127)));
        }
    }
  else
    {
      if (exponent_less_127 == 128)
        /* x is NaN or infinite. */
        return x + x;

      /* All bits in the fraction field are relevant. */
    }
  return x;
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double trunc(double x)
#else
	double trunc(x)
	double x;
#endif
{
	return (double) truncf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
