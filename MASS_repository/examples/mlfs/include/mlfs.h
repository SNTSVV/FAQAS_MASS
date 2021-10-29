/*!
 * -------------------------------------------------------------------------------------------------
 * \file mlfs.h
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Header math.h as defined by ISO C99 standard for the MathlibForSpace.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \author GTD GmbH, Fabian Schriever
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * Header file math.h as defined by the ISO C99 standard. The following procedures are included:
 * sin, asin, cos, acos, tan, atan, atan2, exp, log, log10, fabs, hypot, pow, sqrt, seil, floor,
 * round, trunc, fmod, modf, fmax, fmin, copysign. Additionally there are two more: deg2rad, rem2pi.
 * Each procedure is accompanied by an equivalent version for 32bit precision, they are suffixed
 * with an f, e.g. sinf.
 * The header also includes the macros isfinite, isinf, isnan, and signbit. They work for both 32
 * and 64 bit precision.
 * Also included are the constants as defined by the ISO C99 standard: HUGE_VAL, HUGE_VALF, NAN,
 * INFINITY, MAXFLOAT, and the mathematical constants: M_E, M_LOG2E, M_LOG10E, M_LN2, M_LN10, M_PI,
 * M_PI_2, M_PI_4, M_1_PI, M_2_PI, M_2_SQRTPI, M_SQRT2, M_SQRT1_2.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|----------------------------------------
 * 2016-11-29 | F. Schriever | First creation
 * 2017-06-06 | F. Schriever | Reintroduce __GNUC_PREREQ macro.
 * 2017-06-21 | F. Schriever | Add portability preprocessor if-clauses.
 * 2017-07-31 | F. Schriever | Remove all calls to builtins as the results are not guaranteed to be as expected. Remove no longer needed __GNUC_PREREQ macro.
 * 2017-07-31 | F. Schriever | Add portability preprocessor if-clause for C++.
 * 2017-11-13 | F. Schriever | Add comments with hex-values to constants.
 * 
 * \copyright Copyright European Space Agency, 2017
 * \par License
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 */


#pragma once

#include "mlfs_config.h"

//! Define to force the libm to be built for 64bit doubles.
#ifdef _DOUBLE_IS_32BITS
    #undef _DOUBLE_IS_32BITS
#endif /* _DOUBLE_IS_32BITS */

//! Define to force the libm to be built for only IEEE, no compliance to X/OPEN, POSIX and SVID.
#define _IEEE_LIBM

/**
 * \defgroup mlfs MathlibForSpace
 * @{*/

#ifdef __cplusplus
extern "C"{
#endif

/**
 * \defgroup internals Internals
 *
 * Collection of macros and functions that shall not be used by the user, but are needed by other
 * externally visible macros or functions.
 * @{*/

//! Internal constant used by __fpclassifyd and __fpclassifyf to state the argument was NAN.
#define FP_NAN         0
//! Internal constant used by __fpclassifyd and __fpclassifyf to state the argument was infinite.
#define FP_INFINITE    1
//! Internal constant used by __fpclassifyd and __fpclassifyf to state the argument was zero.
#define FP_ZERO        2
//! Internal constant used by __fpclassifyd and __fpclassifyf to state the argument was subnormal.
#define FP_SUBNORMAL   3
//! Internal constant used by __fpclassifyd and __fpclassifyf to state the argument was normal.
#define FP_NORMAL      4
/*!
 * Internal function used by the fpclassify macro for 64bit doubles.
 *
 * \param[in] x Value to be inspected
 * \return One of the following depending on the input:
 *      FP_ZERO if the argument is positive or negative zero,
 *      FP_NORMAL if the argument is a finite and not subnormal number,
 *      FP_SUBNORMAL if the argument is a subnormal number (exponent is 0),
 *      FP_INFINITE if the argument is positive or negative infinity,
 *      FP_NAN if the argument is none of the above.
 */
extern int __fpclassifyd(double x);
/*!
 * Internal function used by the fpclassify macro for 32bit floats.
 *
 * \param[in] x Value to be inspected
 * \return One of the following depending on the input:
 *      FP_ZERO if the argument is positive or negative zero,
 *      FP_NORMAL if the argument is a finite and not subnormal number,
 *      FP_SUBNORMAL if the argument is a subnormal number (exponent is 0),
 *      FP_INFINITE if the argument is positive or negative infinity,
 *      FP_NAN if the argument is none of the above.
 */
extern int __fpclassifyf(float x);
/*!
 * Macro used to classify the floating point number. It decides whether the number is either zero,
 * normal, subnormal, infinite or NaN.
 *
 * \param[in] x Value to be inspected
 * \return One of the following depending on the input:
 *      FP_ZERO if the argument is positive or negative zero,
 *      FP_NORMAL if the argument is a finite and not subnormal number,
 *      FP_SUBNORMAL if the argument is a subnormal number (exponent is 0),
 *      FP_INFINITE if the argument is positive or negative infinity,
 *      FP_NAN if the argument is none of the above.
 */
#define fpclassify(__x) ((sizeof(__x) == sizeof(float)) ? __fpclassifyf(__x) : __fpclassifyd(__x))

/*!
 * Internal function used by the signbit macro for 64bit doubles.
 *
 * \param[in] x Value to be inspected
 * \return 1 if the argument is negative, 0 if not.
 */
extern int __signbitd(double x);
/*!
 * Internal function used by the signbit macro for 32bit floats.
 *
 * \param[in] x Value to be inspected
 * \return 1 if the argument is negative, 0 if not.
 */
extern int __signbitf(float x);

/**@}*/


/**
 * \defgroup const Constants
 *
 * Collection of constants, including special numbers and mathematical constants.
 * @{*/

/*!
 * Constant for a positive 64bit double
 * Double: 0x7FF00000, 0x00000000
 */
#define HUGE_VAL (1.0e999999999)

/*!
 * Constant for a positive 32bit float
 * Float:  0x7F800000
 */
#define HUGE_VALF (1.0e999999999f)

/*!
 * Constant for positive infinity
 * Float:  0x7F800000
 * Double: 0x7FF00000, 0x00000000
 */
#define INFINITY (HUGE_VALF)

//! Constant for not a number
#define NAN (0.0F/0.0F)


/*!
 * Constant defining the maximum finite positive value a 32bit float can represent.
 * Float:  0x7F7FFFFF
 */
#define MAXFLOAT    (3.40282347e+38F)


/*!
 * Constant for e
 * Float:  0x402DF854
 * Double: 0x4005BF0A, 0x8B145769
 */
#define M_E         (2.7182818284590452354)

/*!
 * Constant for base 2 logarithm of e
 * Float:  0x3FB8AA3B
 * Double: 0x3FF71547, 0x652B82FE
 */
#define M_LOG2E     (1.4426950408889634074)

/*!
 * Constant for base 10 logarithm of e
 * Float:  0x3EDE5BD9
 * Double: 0x3FDBCB7B, 0x1526E50E
 */
#define M_LOG10E    (0.43429448190325182765)

/*!
 * Constant for base e logarithm of 2
 * Float:  0x3F317218
 * Double: 0x3FE62E42, 0xFEFA39EF
 */
#define M_LN2       (0.693147180559945309417)

/*!
 * Constant for base e logarithm of 10
 * Float:  0x40135D8E
 * Double: 0x40026BB1, 0xBBB55516
 */
#define M_LN10      (2.30258509299404568402)

/*!
 * Constant for pi
 * Float:  0x40490FDB
 * Double: 0x400921FB, 0x54442D18
 */
#define M_PI        (3.14159265358979323846)

/*!
 * Constant for pi/2
 * Float:  0x3FC90FDB
 * Double: 0x3FF921FB, 0x54442D18
 */
#define M_PI_2      (1.57079632679489661923)

/*!
 * Constant for pi/4
 * Float:  0x3F490FDB
 * Double: 0x3FE921FB, 0x54442D18
 */
#define M_PI_4      (0.78539816339744830962)

/*!
 * Constant for 1/pi
 * Float:  0x3EA2F983
 * Double: 0x3FD45F30, 0x6DC9C883
 */
#define M_1_PI      (0.31830988618379067154)

/*!
 * Constant for 2/pi
 * Float:  0x3F22F983
 * Double: 0x3FE45F30, 0x6DC9C883
 */
#define M_2_PI      (0.63661977236758134308)

/*!
 * Constant for 2/sqrt(pi)
 * Float:  0x3F906EBB
 * Double: 0x3FF20DD7, 0x50429B6D
 */
#define M_2_SQRTPI  (1.12837916709551257390)

/*!
 * Constant for sqrt(2)
 * Float:  0x3FB504F3
 * Double: 0x3FF6A09E, 0x667F3BCD
 */
#define M_SQRT2     (1.41421356237309504880)

/*!
 * Constant for sqrt(1/2)
 * Float:  0x3F3504F3
 * Double: 0x3FE6A09E, 0x667F3BCD
 */
#define M_SQRT1_2   (0.70710678118654752440)

/**@}*/


/**
 * \defgroup dpp Double Precision Procedures
 *
 * Collection of mathematical procedures to be used with 64bit double precision.
 * @{*/

/*!
 * Converts the argument given in degrees to radians.
 * 64bit double version.
 *
 * \param[in] x Degree value to be converted.
 * \return Radians value of the argument.
 */
extern double deg2rad(const double x);

/*!
 * Reduces the argument to the range [+0, 2pi] such that
 * 0 <= |x - (n * 2pi)| <= 2pi
 * with n being an integer value.
 * 64bit double version.
 *
 * \param[in] x Value to be reduced.
 * \return Reduced value.
 */
extern double rem2pi(const double x);


/*!
 * Evaluates the sine of the argument given in radians.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Sine of the argument.
 */
extern double sin(const double x);

/*!
 * Evaluates the inverse to the sine of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Inverse to the sine of the argument in radians.
 */
extern double asin(const double x);

/*!
 * Evaluates the cosine of the argument given in radians.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Cosine of the argument.
 */
extern double cos(const double x);

/*!
 * Evaluates the inverse to the cosine of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Inverse to the cosine of the argument in radians.
 */
extern double acos(const double x);

/*!
 * Evaluates the tangent of the argument given in radians.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Tangent of the argument.
 */
extern double tan(const double x);

/*!
 * Evaluates the inverse tangent of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Inverse to the tangent of the argument in radians.
 */
extern double atan(const double x);

/*!
 * Evaluates the inverse tangent of y/x in the correct quadrant.
 * 64bit double version.
 *
 * \param[in] y Dividend argument value.
 * \param[in] x Divisor argument value.
 * \return Inverse to the tangent of y/x in the correct quadrant, in radians.
 */
extern double atan2(const double y, const double x);


/*!
 * Evaluates the base e exponential of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Base e exponential of the argument.
 */
extern double exp(const double x);

/*!
 * Evaluates the base e logarithm (natural logarithm) of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Base e logarithm of the argument.
 */
extern double log(const double x);

/*!
 * Evaluates the base 10 logarithm of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Base 10 logarithm of the argument.
 */
extern double log10(const double x);


/*!
 * Evaluates the absolute value of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Absolute value of the argument.
 */
extern double fabs(const double x);

/*!
 * Evaluates the hypotenuse of a right-angled triangle with the shorter sides given as arguments.
 * 64bit double version.
 *
 * \param[in] x Right-angled triangle side argument value.
 * \param[in] y Right-angled triangle side argument value.
 * \return Hypotenuse of the right-angled triangle with the shorter sides x and y.
 */
extern double hypot(const double x, const double y);

/*!
 * Evaluates the base x exponential of the argument y.
 * 64bit double version.
 *
 * \param[in] x Base argument value.
 * \param[in] y Exponent argument value.
 * \return Base x exponential of y.
 */
extern double pow(const double x, const double y);

/*!
 * Evaluates the square root of the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return Square root of the argument.
 */
extern double sqrt(const double x);


/*!
 * Evaluates the argument rounded to the smallest integer value not less than the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the smallest integer value not less than the argument.
 */
extern double ceil(const double x);

/*!
 * Evaluates the argument rounded to the largest integer value not greater than the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the largest integer value not greater than the argument.
 */
extern double floor(const double x);

/*!
 * Evaluates the argument rounded to the nearest integer value, rounding edge cases away from zero.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the nearest integer value, rounding edge cases away from zero.
 */
extern double round(const double x);

/*!
 * Evaluates the argument rounded to the nearest integer value, but not greater in magnitude than
 * the argument.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the nearest integer value, but not greater in magnitude than
 * the argument.
 */
extern double trunc(const double x);


/*!
 * Evaluates the floating point remainder of x divided by y.
 * 64bit double version.
 *
 * \param[in] x Dividend argument value.
 * \param[in] y Divisor argument value.
 * \return The floating point remainder of x divided by y.
 */
extern double fmod(const double x, const double y);

/*!
 * Splits the argument x into an integral and a fractional part.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \param[in] iPtr Pointer to return the integral part of the argument x.
 * \return The fractional part of the argument x.
 */
extern double modf(const double x, double * const iptr);


/*!
 * Evaluates the larger of the two arguments.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \param[in] y Argument value.
 * \return The larger of the two arguments.
 */
extern double fmax(const double x, const double y);

/*!
 * Evaluates the smaller of the two arguments.
 * 64bit double version.
 *
 * \param[in] x Argument value.
 * \param[in] y Argument value.
 * \return The smaller of the two arguments.
 */
extern double fmin(const double x, const double y);


/*!
 * Evaluates the value with the magnitude of x and the sign of y.
 * 64bit double version.
 *
 * \param[in] x Argument value providing the magnitude.
 * \param[in] y Argument value providing the sign.
 * \return Value with the magnitude of x and the sign of y.
 */
extern double copysign(const double x, const double y);

/**@}*/


/**
 * \defgroup spp Single Precision Procedures
 *
 * Collection of mathematical procedures to be used with 32bit single precision.
 * @{*/

/*!
 * Converts the argument given in degrees to radians.
 * 32bit float version.
 *
 * \param[in] x Degree value to be converted.
 * \return Radians value of the argument.
 */
extern float deg2radf(const float x);

/*!
 * Reduces the argument to the range [+0, 2pi] such that
 * 0 <= |x - (n * 2pi)| <= 2pi
 * with n being an integer value.
 * 32bit float version.
 *
 * \param[in] x Value to be reduced.
 * \return Reduced value.
 */
extern float rem2pif(const float x);


/*!
 * Evaluates the sine of the argument given in radians.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Sine of the argument.
 */
extern float sinf(const float x);

/*!
 * Evaluates the inverse to the sine of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Inverse to the sine of the argument in radians.
 */
extern float asinf(const float x);

/*!
 * Evaluates the cosine of the argument given in radians.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Cosine of the argument.
 */
extern float cosf(const float x);

/*!
 * Evaluates the inverse to the cosine of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Inverse to the cosine of the argument in radians.
 */
extern float acosf(const float x);

/*!
 * Evaluates the tangent of the argument given in radians.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Tangent of the argument.
 */
extern float tanf(const float x);

/*!
 * Evaluates the inverse tangent of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Inverse to the tangent of the argument in radians.
 */
extern float atanf(const float x);

/*!
 * Evaluates the inverse tangent of y/x in the correct quadrant.
 * 32bit float version.
 *
 * \param[in] y Dividend argument value.
 * \param[in] x Divisor argument value.
 * \return Inverse to the tangent of y/x in the correct quadrant, in radians.
 */
extern float atan2f(const float y, const float x);


/*!
 * Evaluates the base e exponential of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Base e exponential of the argument.
 */
extern float expf(const float x);

/*!
 * Evaluates the base e logarithm (natural logarithm) of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Base e logarithm of the argument.
 */
extern float logf(const float x);

/*!
 * Evaluates the base 10 logarithm of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Base 10 logarithm of the argument.
 */
extern float log10f(const float x);


/*!
 * Evaluates the absolute value of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Absolute value of the argument.
 */
extern float fabsf(const float x);

/*!
 * Evaluates the hypotenuse of a right-angled triangle with the shorter sides given as arguments.
 * 32bit float version.
 *
 * \param[in] x Right-angled triangle side argument value.
 * \param[in] y Right-angled triangle side argument value.
 * \return Hypotenuse of the right-angled triangle with the shorter sides x and y.
 */
extern float hypotf(const float x, const float y);

/*!
 * Evaluates the base x exponential of the argument y.
 * 32bit float version.
 *
 * \param[in] x Base argument value.
 * \param[in] y Exponent argument value.
 * \return Base x exponential of y.
 */
extern float powf(const float x, const float y);

/*!
 * Evaluates the square root of the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return Square root of the argument.
 */
extern float sqrtf(const float x);


/*!
 * Evaluates the argument rounded to the smallest integer value not less than the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the smallest integer value not less than the argument.
 */
extern float ceilf(const float x);

/*!
 * Evaluates the argument rounded to the largest integer value not greater than the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the largest integer value not greater than the argument.
 */
extern float floorf(const float x);

/*!
 * Evaluates the argument rounded to the nearest integer value, rounding edge cases away from zero.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the nearest integer value, rounding edge cases away from zero.
 */
extern float roundf(const float x);

/*!
 * Evaluates the argument rounded to the nearest integer value, but not greater in magnitude than
 * the argument.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \return The argument rounded to the nearest integer value, but not greater in magnitude than
 * the argument.
 */
extern float truncf(const float x);


/*!
 * Evaluates the floating point remainder of x divided by y.
 * 32bit float version.
 *
 * \param[in] x Dividend argument value.
 * \param[in] y Divisor argument value.
 * \return The floating point remainder of x divided by y.
 */
extern float fmodf(const float x, const float y);

/*!
 * Splits the argument x into an integral and a fractional part.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \param[in] iPtr Pointer to return the integral part of the argument x.
 * \return The fractional part of the argument x.
 */
extern float modff(const float x, float * const iptr);


/*!
 * Evaluates the larger of the two arguments.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \param[in] y Argument value.
 * \return The larger of the two arguments.
 */
extern float fmaxf(const float x, const float y);

/*!
 * Evaluates the smaller of the two arguments.
 * 32bit float version.
 *
 * \param[in] x Argument value.
 * \param[in] y Argument value.
 * \return The smaller of the two arguments.
 */
extern float fminf(const float x, const float y);


/*!
 * Evaluates the value with the magnitude of x and the sign of y.
 * 32bit float version.
 *
 * \param[in] x Argument value providing the magnitude.
 * \param[in] y Argument value providing the sign.
 * \return Value with the magnitude of x and the sign of y.
 */
extern float copysignf(const float x, const float y);

/**@}*/


/**
 * \defgroup macros Macros
 *
 * Collection of macros to classify the input value. All macros can be used with both
 * single and double precision.
 * @{*/

/*!
 * Macro returning whether the argument is neither infinite nor NaN.
 *
 * \param[in] x Value to be inspected
 * \return 1 if the argument is neither infinite nor NaN, 0 if not.
 */
#define isfinite(__y) (__extension__ ({int __cy = fpclassify(__y); \
                                       __cy != FP_INFINITE && __cy != FP_NAN;}))

/*!
 * Macro returning whether the argument is positive or negative infinity.
 *
 * \param[in] x Value to be inspected
 * \return 1 if the argument is infinite, 0 if not.
 */
#define isinf(__x) (fpclassify(__x) == FP_INFINITE)

/*!
 * Macro returning whether the argument is not a number.
 *
 * \param[in] x Value to be inspected
 * \return 1 if the argument is not a number, 0 if it is.
 */
#define isnan(__x) (fpclassify(__x) == FP_NAN)


/*!
 * Macro returning whether the argument's signbit is set.
 *
 * \param[in] x Value to be inspected
 * \return 1 if the argument is negative, 0 if not.
 */
#define signbit(__x) ((sizeof(__x) == sizeof(float)) ? __signbitf(__x) : __signbitd (__x))

/**@}*/

#ifdef __cplusplus
}
#endif

/**@}*/
