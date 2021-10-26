/*!
 * -------------------------------------------------------------------------------------------------
 * \file kf_tan.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for kernel tanf.
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
 * -----------|--------------|-----------------------------------------------
 * 2017-08-17 | F. Schriever | Move optimisation for tiny x from kernel to external procedure, according to https://github.com/freebsd/freebsd/commit/ea9959fde3470076f9f0784a1b2a6b07e29fc535
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* kf_tan.c -- float version of k_tan.c
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
#ifdef __STDC__
static const float 
#else
static float 
#endif
pio4  =  7.8539812565e-01f, /* 0x3f490fda */
pio4lo=  3.7748947079e-08f, /* 0x33222168 */
T[] =  {
  3.3333334327e-01f, /* 0x3eaaaaab */
  1.3333334029e-01f, /* 0x3e088889 */
  5.3968254477e-02f, /* 0x3d5d0dd1 */
  2.1869488060e-02f, /* 0x3cb327a4 */
  8.8632395491e-03f, /* 0x3c11371f */
  3.5920790397e-03f, /* 0x3b6b6916 */
  1.4562094584e-03f, /* 0x3abede48 */
  5.8804126456e-04f, /* 0x3a1a26c8 */
  2.4646313977e-04f, /* 0x398137b9 */
  7.8179444245e-05f, /* 0x38a3f445 */
  7.1407252108e-05f, /* 0x3895c07a */
 -1.8558637748e-05f, /* 0xb79bae5f */
  2.5907305826e-05f, /* 0x37d95384 */
};

#ifdef __STDC__
	float __kernel_tanf(float x, float y, __int32_t iy)
#else
	float __kernel_tanf(x, y, iy)
	float x,y; __int32_t iy;
#endif
{
	float z,r,v,w,s;
	__int32_t ix,hx;
	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;	/* high word of |x| */
	if(ix>=0x3f2ca140) { 			/* |x|>=0.6744 */
	    if(hx<0) {x = -x; y = -y;}
	    z = pio4-x;
	    w = pio4lo-y;
	    x = z+w; y = 0.0f;
	}
	z	=  x*x;
	w 	=  z*z;
    /* Break x^5*(T[1]+x^2*T[2]+...) into
     *	  x^5(T[1]+x^4*T[3]+...+x^20*T[11]) +
     *	  x^5(x^2*(T[2]+x^4*T[4]+...+x^22*[T12]))
     */
	r = T[1]+w*(T[3]+w*(T[5]+w*(T[7]+w*(T[9]+w*T[11]))));
	v = z*(T[2]+w*(T[4]+w*(T[6]+w*(T[8]+w*(T[10]+w*T[12])))));
	s = z*x;
	r = y + z*(s*(r+v)+y);
	r += T[0]*s;
	w = x+r;
	if(ix>=0x3f2ca140) {
	    v = (float)iy;
	    return (float)(1-((hx>>30)&2))*(v-(float)2.0*(x-(w*w/(w+v)-r)));
	}
	if(iy==1) return w;
	else {		/* if allow error up to 2 ulp, 
			   simply return -1.0/(x+r) here */
     /*  compute -1.0/(x+r) accurately */
	    float a,t;
	    __int32_t i;
	    z  = w;
	    GET_FLOAT_WORD(i,z);
	    SET_FLOAT_WORD(z,i&0xfffff000U);
	    v  = r-(z - x); 	/* z+v = r+x */
	    t = a  = -(float)1.0/w;	/* a = -1.0/w */
	    GET_FLOAT_WORD(i,t);
	    SET_FLOAT_WORD(t,i&0xfffff000U);
	    s  = (float)1.0+t*z;
	    return t+a*(s+t*v);
	}
}
