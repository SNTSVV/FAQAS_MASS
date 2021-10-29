/*!
 * -------------------------------------------------------------------------------------------------
 * \file ef_exp.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for the expf procedure.
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
 * -----------|--------------|----------------------------------------------------------
 * 2017-08-16 | F. Schriever | Adjust constants ln2HI/LO according to https://github.com/freebsd/freebsd/commit/06d803185574a0ba37434b1e98b322e54cc32a4e
 * 2017-08-18 | F. Schriever | Make variables huge and twom100 volatile to enforce correct behaviour with regards to exceptions (otherwise the calculation may no longer exist after compilation due to optimization).
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* ef_exp.c -- float version of e_exp.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifdef __v810__
#define const
#endif

static volatile float
huge    = 1.0e+30f,
twom100 = 7.8886090522e-31f;      /* 2**-100=0x0d800000 */

#ifdef __STDC__
static const float
#else
static float
#endif
one	= 1.0f,
halF[2]	= {0.5f,-0.5f,},
ln2HI[2] ={ 6.9314575195e-01f,     /* 0x3f317200 */
           -6.9314575195e-01f,},   /* 0xbf317200 */
ln2LO[2] ={ 1.4286067653e-06f,     /* 0x35bfbe8e */
           -1.4286067653e-06f,},   /* 0xb5bfbe8e */
invln2 =  1.4426950216e+00f, 		/* 0x3fb8aa3b */
P1   =  1.6666667163e-01f, /* 0x3e2aaaab */
P2   = -2.7777778450e-03f, /* 0xbb360b61 */
P3   =  6.6137559770e-05f, /* 0x388ab355 */
P4   = -1.6533901999e-06f, /* 0xb5ddea0e */
P5   =  4.1381369442e-08f; /* 0x3331bb4c */

#ifdef __STDC__
	float __ieee754_expf(float x)	/* default IEEE float exp */
#else
	float __ieee754_expf(x)	/* default IEEE float exp */
	float x;
#endif
{
	float y,hi=0.0f,lo=0.0f,c,t;
	__int32_t k = 0,xsb,sx;
	__uint32_t hx;

	GET_FLOAT_WORD(sx,x);
	xsb = (sx>>31)&1;		/* sign bit of x */
	hx = sx & 0x7fffffff;		/* high word of |x| */

    /* filter out non-finite argument */
        if(FLT_UWORD_IS_NAN(hx))
            return x+x;	 	/* NaN */
        if(FLT_UWORD_IS_INFINITE(hx))
	    return (xsb==0)? x:0.0f;		/* exp(+-inf)={inf,0} */
	if(sx > FLT_UWORD_LOG_MAX)
	    return huge*huge; /* overflow */
	if(sx < 0 && hx > FLT_UWORD_LOG_MIN)
	    return twom100*twom100; /* underflow */
	
    /* argument reduction */
	if(hx > 0x3eb17218) {		/* if  |x| > 0.5 ln2 */ 
	    if(hx < 0x3F851592) {	/* and |x| < 1.5 ln2 */
		hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = 1-xsb-xsb;
	    } else {
		k  = invln2*x+halF[xsb];
		t  = k;
		hi = x - t*ln2HI[0];	/* t*ln2HI is exact here */
		lo = t*ln2LO[0];
	    }
	    x  = hi - lo;
	} 
	else if(hx < 0x31800000)  {	/* when |x|<2**-28 */
	    if(huge+x>one) return one+x;/* trigger inexact */
	}

    /* x is now in primary range */
	t  = x*x;
	c  = x - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
	if(k==0) 	return one-((x*c)/(c-(float)2.0)-x); 
	else 		y = one-((lo-(x*c)/((float)2.0-c))-hi);
	if(k >= -125) {
	    __uint32_t hy;
	    GET_FLOAT_WORD(hy,y);
	    SET_FLOAT_WORD(y,hy+(k<<23));	/* add k to y's exponent */
	    return y;
	} else {
	    __uint32_t hy;
	    GET_FLOAT_WORD(hy,y);
	    SET_FLOAT_WORD(y,hy+((k+100)<<23));	/* add k to y's exponent */
	    return y*twom100;
	}
}
