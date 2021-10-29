/*!
 * -------------------------------------------------------------------------------------------------
 * \file sf_fmax.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for fmax.
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

/* Copyright (C) 2002 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include "fdlibm.h"

#ifdef __STDC__
	float fmaxf(float x, float y)
#else
	float fmaxf(x,y)
	float x;
	float y;
#endif
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_onef;
  y *= __volatile_onef;
#endif /* MLFS_FPU_DAZ */

  if (__fpclassifyf(x) == FP_NAN)
    return y;
  if (__fpclassifyf(y) == FP_NAN)
    return x;
  
  return x > y ? x : y;
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double fmax(double x, double y)
#else
	double fmax(x,y)
	double x;
	double y;
#endif
{
  return (double) fmaxf((float) x, (float) y);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
