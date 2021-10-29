/*!
 * -------------------------------------------------------------------------------------------------
 * \file s_nan.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file of the nanf functionality.
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
 * nanf () returns a nan.
 * Added by Cygnus Support.
 */

#include "fdlibm.h"

	float nanf(const char *unused)
{
	float x;

	(void)unused;

	SET_FLOAT_WORD(x,0x7fc00000);

	return x;
}

#ifdef _DOUBLE_IS_32BITS

	double nan(const char *arg)
{
	return (double) nanf(arg);
}

#endif /* defined(_DOUBLE_IS_32BITS) */

