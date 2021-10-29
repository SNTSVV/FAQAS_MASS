/*!
 * -------------------------------------------------------------------------------------------------
 * \file kf_log.h
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for __kernel_logf.
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
 * 2017-08-17 | F. Schriever | Initial implementation, based on the FreeBSD commit https://github.com/freebsd/freebsd/commit/225ceb18e8a1729edd452bb883adfdb72d75c769 and modified with commit https://github.com/freebsd/freebsd/commit/2772ec758e6fb6c0bdaa3e30a268816e754de5a7
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
 * Float version of k_log.h.  See the latter for most comments.
 */

static const float
/* |(log(1+s)-log(1-s))/s - Lg(s)| < 2**-34.24 (~[-4.95e-11, 4.97e-11]). */
Lg1 =      0xaaaaaa.0p-24,  /* 0.66666662693, 0x3F2AAAAA */
Lg2 =      0xccce13.0p-25,  /* 0.40000972152, 0x3ECCCE13 */
Lg3 =      0x91e9ee.0p-25,  /* 0.28498786688, 0x3E91E9EE */
Lg4 =      0xf89e26.0p-26;  /* 0.24279078841, 0x3E789E26 */

static inline float
__kernel_logf(float f)
{
    float hfsq,s,z,R,w,t1,t2;

    s = f/((float)2.0+f);
    z = s*s;
    w = z*z;
    t1= w*(Lg2+w*Lg4);
    t2= z*(Lg1+w*Lg3);
    R = t2+t1;
    hfsq=(float)0.5*f*f;
    return s*(hfsq+R);
}
