/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_nan.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file of the nan functionality.
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
 * 2017-07-31 | F. Schriever | Remove use of __builtin_nan.
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/*
 * nan () returns a nan.
 * Added by Cygnus Support.
 */

/*
FUNCTION
	<<nan>>, <<nanf>>---representation of ``Not a Number''

INDEX
	nan
INDEX
	nanf

ANSI_SYNOPSIS
	#include <math.h>
	double nan(const char *<[unused]>);
	float nanf(const char *<[unused]>);

TRAD_SYNOPSIS
	#include <math.h>
	double nan();
	float nanf();


DESCRIPTION
	<<nan>> and <<nanf>> return an IEEE NaN (Not a Number) in
	double- and single-precision arithmetic respectively.  The
	argument is currently disregarded.

QUICKREF
	nan - pure

*/

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

	double nan(const char *unused)
{
	double x;

	(void)unused;

	INSERT_WORDS(x,0x7ff80000,0);
	return x;
}

#endif /* _DOUBLE_IS_32BITS */
