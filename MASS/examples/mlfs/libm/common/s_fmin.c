/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_fmin.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for fmin.
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
/*
FUNCTION
<<fmin>>, <<fminf>>---minimum
INDEX
	fmin
INDEX
	fminf

ANSI_SYNOPSIS
	#include <math.h>
	double fmin(double <[x]>, double <[y]>);
	float fminf(float <[x]>, float <[y]>);

DESCRIPTION
The <<fmin>> functions determine the minimum numeric value of their arguments.
NaN arguments are treated as missing data:  if one argument is a NaN and the
other numeric, then the <<fmin>> functions choose the numeric value.

RETURNS
The <<fmin>> functions return the minimum numeric value of their arguments.

PORTABILITY
ANSI C, POSIX.

*/

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double fmin(double x, double y)
#else
	double fmin(x,y)
	double x;
	double y;
#endif
{
#ifdef MLFS_FPU_DAZ
  x *= __volatile_one;
  y *= __volatile_one;
#endif /* MLFS_FPU_DAZ */

  if (__fpclassifyd(x) == FP_NAN)
    return y;
  if (__fpclassifyd(y) == FP_NAN)
    return x;
  
  return x < y ? x : y;
}

#endif /* _DOUBLE_IS_32BITS */
