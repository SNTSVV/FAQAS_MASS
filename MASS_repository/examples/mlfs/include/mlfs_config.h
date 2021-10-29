/*!
 * -------------------------------------------------------------------------------------------------
 * \file mlfs_config.h
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Configuration file for the MathlibForSpace.
 * \version c07acbf539f8880d8c1255e7f85138b89f33387b
 * \date 2018-03-22 17:44:24 +0100
 * \author GTD GmbH, Fabian Schriever
 * \par Last Commit
 * Fabian Schriever, 2018-03-22 17:44:24 +0100
 *
 * \par Detailed Description
 * This file can be used to change the configuration of the MathlibForSpace.
 * It may be used to define the behaviour of assertions within the code.
 *
 * -------------------------------------------------------------------------------------------------
 * \par Changelog
 * Date       | Name         | Change
 * -----------|--------------|---------------
 * 2017-06-06 | F. Schriever | First creation
 * 
 * \copyright Copyright European Space Agency, 2017
 * \par License
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 */


#pragma once

/* If an FPU is not standard compliant for subnormal values, use the define MLFS_FPU_DAZ to force 
 * each procedure to first multiply the input value(s) by 1, therefore using the FPUs defined
 * behaviour for subnormal values as follows:
 * If the FPU is standard compliant for subnormals, the multiplication does not cause any
 * difference in the behaviour of the procedure.
 * If the FPU can not handle subnormals at all, it's very likely that a special FPU trap will be
 * thrown. This trap can then be handled by the user if so inclined.
 * If the FPU has an internal implementation for DAZ (denormals are zero), the input values will be
 * reduced to zero and the procedures will then behave as if a zero was used as an input. Usually
 * this will also generate an inexact exception. */
// #define MLFS_FPU_DAZ
