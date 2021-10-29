/*!
 * -------------------------------------------------------------------------------------------------
 * \file fdlibm.h
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Header file of the fdlibm library.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * This header file contains prototypes for internal procedures. It also houses several macros to
 * manipulate words of 4 bytes.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|----------------------------------------------------------------------
 * 2016-11-29 | F. Schriever | Replace includes with mlfs includes.
 * 2016-11-29 | F. Schriever | Add include guard.
 * 2016-11-29 | F. Schriever | Remove #define _XOPEN_MODE as mlfs is for IEEE.
 * 2016-11-29 | F. Schriever | Add if-clause to get word order.
 * 2017-05-19 | F. Schriever | Remove function and macro declarations that are not part of the MLFS.
 * 2017-06-21 | F. Schriever | Remove argument prec from __kernel_rem_pio2/f
 * 2017-07-31 | F. Schriever | Remove argument ipio2 from __kernel_rem_pio2/f
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* @(#)fdlibm.h 5.1 93/09/24 */
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

#pragma once

#include <stdbool.h>
#include <mlfs.h>
#include <sys/types.h>

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define __IEEE_BIG_ENDIAN
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define __IEEE_LITTLE_ENDIAN
#endif

//! _HAVE_STDC macro is used by some of the wrappers. Some RTEMS TCs already define this macro.
#if !defined(_HAVE_STDC)
    #define _HAVE_STDC __STDC__
#endif

/* When MLFS_FPU_DAZ is defined, all input values shall be multiplied with __volatile_one to force
 * FPU usage, which in turn causes the input values to be as the FPU itself handles subnormals.
 * This can cause  */
#ifdef MLFS_FPU_DAZ
static volatile double __volatile_one = 1.0;
static volatile float __volatile_onef = 1.0f;
#endif /* MLFS_FPU_DAZ */

/* Most routines need to check whether a float is finite, infinite, or not a
   number, and many need to know whether the result of an operation will
   overflow. The macros below wrap up that kind of information:

   FLT_UWORD_IS_FINITE(X)
	True if a positive float with bitmask X is finite.

   FLT_UWORD_IS_NAN(X)
	True if a positive float with bitmask X is not a number.

   FLT_UWORD_IS_INFINITE(X)
	True if a positive float with bitmask X is +infinity.

   FLT_UWORD_MAX
	The bitmask of FLT_MAX.

   FLT_UWORD_EXP_MAX
	The bitmask of the largest finite exponent.

   FLT_UWORD_LOG_MAX
	The bitmask of log(FLT_MAX), rounded down.  This value is the largest
	input that can be passed to exp() without producing overflow.

   FLT_LARGEST_EXP
	The largest biased exponent that can be used for finite numbers. */

#define FLT_UWORD_IS_FINITE(x) ((x)<0x7f800000L)
#define FLT_UWORD_IS_NAN(x) ((x)>0x7f800000L)
#define FLT_UWORD_IS_INFINITE(x) ((x)==0x7f800000L)
#define FLT_UWORD_MAX 0x7f7fffffL
#define FLT_UWORD_EXP_MAX 0x43000000
#define FLT_UWORD_LOG_MAX 0x42b17217
#define HUGE ((float)3.40282346638528860e+38)
#define FLT_LARGEST_EXP (FLT_UWORD_MAX>>23)

/* Many routines check for zero and subnormal numbers:

   FLT_UWORD_IS_ZERO(X)
	True if a positive float with bitmask X is +0.

   FLT_UWORD_IS_SUBNORMAL(X)
	True if a non-zero positive float with bitmask X is subnormal.

   FLT_UWORD_EXP_MIN
	The bitmask of the float representation of REAL_FLT_MIN's exponent.

   FLT_UWORD_LOG_MIN
	The bitmask of |log(REAL_FLT_MIN)|, rounding down.

   FLT_SMALLEST_EXP
	REAL_FLT_MIN's exponent - EXP_BIAS.
*/

#define FLT_UWORD_IS_ZERO(x) ((x)==0)
#define FLT_UWORD_IS_SUBNORMAL(x) ((x)<0x00800000L)
#define FLT_UWORD_EXP_MIN 0x43160000
#define FLT_UWORD_LOG_MIN 0x42cff1b5
#define FLT_SMALLEST_EXP -22

/* formerly external functions */
extern double nan(const char * unused);
extern double scalbn(double x, int n);

extern float nanf(const char * unused);
extern float scalbnf(float x, int n);

/* ieee style elementary functions */
extern double __ieee754_sqrt(double x);
extern double __ieee754_acos(double x);
extern double __ieee754_log(double x);
extern double __ieee754_log10(double x);
extern double __ieee754_asin(double x);
extern double __ieee754_atan2(double y, double x);
extern double __ieee754_exp(double x);
extern double __ieee754_fmod(double x, double y);
extern double __ieee754_pow(double x, double y);
extern double __ieee754_hypot(double x, double y);
extern __int32_t __ieee754_rem_pio2(double x, double * y);

/* fdlibm kernel function */
extern double __kernel_sin(double x, double y, __int32_t iy);
extern double __kernel_cos(double x, double y);
extern double __kernel_tan(double x, double y, __int32_t iy);
extern __int32_t __kernel_rem_pio2(double *x, double *y, __int32_t e0, __int32_t nx);

/* ieee style elementary float functions */
extern float __ieee754_sqrtf(float x);
extern float __ieee754_acosf(float x);
extern float __ieee754_logf(float x);
extern float __ieee754_log10f(float x);
extern float __ieee754_asinf(float x);
extern float __ieee754_atan2f(float y, float x);
extern float __ieee754_expf(float x);
extern float __ieee754_fmodf(float x, float y);
extern float __ieee754_powf(float x, float y);
extern float __ieee754_hypotf(float x, float y);
extern __int32_t __ieee754_rem_pio2f(float x, float * y);

/* float versions of fdlibm kernel functions */
extern float __kernel_sinf(float x, float y, __int32_t iy);
extern float __kernel_cosf(float x, float y);
extern float __kernel_tanf(float x, float y, __int32_t iy);
extern __int32_t __kernel_rem_pio2f(float *x, float *y, __int32_t e0, __int32_t nx);

/* The original code used statements like
	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
	ix0 = *(n0+(int*)&x);			* high word of x *
	ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

#ifndef __IEEE_BIG_ENDIAN
#ifndef __IEEE_LITTLE_ENDIAN
 #error Must define endianness
#endif
#endif

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

#ifdef __IEEE_BIG_ENDIAN

typedef union
{
  double value;
  struct
  {
    __uint32_t msw;
    __uint32_t lsw;
  } parts;
} ieee_double_shape_type;

#endif

#ifdef __IEEE_LITTLE_ENDIAN

typedef union
{
  double value;
  struct
  {
    __uint32_t lsw;
    __uint32_t msw;
  } parts;
} ieee_double_shape_type;

#endif

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (false)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
do {								\
  ieee_double_shape_type gh_u;					\
  gh_u.value = (d);						\
  (i) = gh_u.parts.msw;						\
} while (false)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
do {								\
  ieee_double_shape_type gl_u;					\
  gl_u.value = (d);						\
  (i) = gl_u.parts.lsw;						\
} while (false)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)					\
do {								\
  ieee_double_shape_type iw_u;					\
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (false)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)					\
do {								\
  ieee_double_shape_type sh_u;					\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (false)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)					\
do {								\
  ieee_double_shape_type sl_u;					\
  sl_u.value = (d);						\
  sl_u.parts.lsw = (v);						\
  (d) = sl_u.value;						\
} while (false)

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  __uint32_t word;
} ieee_float_shape_type;

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
do {								\
  ieee_float_shape_type gf_u;					\
  gf_u.value = (d);						\
  (i) = gf_u.word;						\
} while (false)

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
do {								\
  ieee_float_shape_type sf_u;					\
  sf_u.word = (i);						\
  (d) = sf_u.value;						\
} while (false)
