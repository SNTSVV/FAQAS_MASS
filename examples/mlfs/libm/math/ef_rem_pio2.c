/*!
 * -------------------------------------------------------------------------------------------------
 * \file ef_rem_pio2.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for internal range reduction.
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
 * -----------|--------------|-----------------------------------------------------------
 * 2017-06-09 | F. Schriever | Replace possibly not ending while-loop with for-loop
 * 2017-06-21 | F. Schriever | Remove input variable prec from call to __kernel_rem_pio2f
 * 2017-07-31 | F. Schriever | Move array two_over_pi to __kernel_rem_pio2f
 * 2017-08-17 | F. Schriever | Fix reduction near (but not very near) +-pi/2, according to https://github.com/freebsd/freebsd/commit/13f78201bbddb9bf6e0575ef8ccece4c86948a97, and modified by https://github.com/freebsd/freebsd/commit/485c06b5bb6bff2405d6e5eee8df62b73cf5e3ae
 * 2017-08-17 | F. Schriever | Remove no cancellation optimisation, similar to https://github.com/freebsd/freebsd/commit/e31bf4b688423b9d648923a471ed8a41940b9432
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* ef_rem_pio2.c -- float version of e_rem_pio2.c
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
 *
 */

/* __ieee754_rem_pio2f(x,y)
 * 
 * return the remainder of x rem pi/2 in y[0]+y[1] 
 * use __kernel_rem_pio2f()
 */

#include "fdlibm.h"

/*
 * invpio2:  24 bits of 2/pi
 * pio2_1:   first  17 bit of pi/2
 * pio2_1t:  pi/2 - pio2_1
 * pio2_2:   second 17 bit of pi/2
 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
 * pio2_3:   third  17 bit of pi/2
 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
 */

#ifdef __STDC__
static const float 
#else
static float 
#endif
zero =  0.0000000000e+00f, /* 0x00000000 */
half =  5.0000000000e-01f, /* 0x3f000000 */
two8 =  2.5600000000e+02f, /* 0x43800000 */
invpio2 =  6.3661980629e-01f, /* 0x3f22f984 */
pio2_1  =  1.5707855225e+00f, /* 0x3fc90f80 */
pio2_1t =  1.0804334124e-05f, /* 0x37354443 */
pio2_2  =  1.0804273188e-05f, /* 0x37354400 */
pio2_2t =  6.0770999344e-11f, /* 0x2e85a308 */
pio2_3  =  6.0770943833e-11f, /* 0x2e85a300 */
pio2_3t =  6.1232342629e-17f; /* 0x248d3132 */

#ifdef __STDC__
	__int32_t __ieee754_rem_pio2f(float x, float *y)
#else
	__int32_t __ieee754_rem_pio2f(x,y)
	float x,y[];
#endif
{
	float z,w,t,r,fn;
	float tx[3];
	__int32_t i,j,n,ix,hx;
	__int32_t e0,nx;

	GET_FLOAT_WORD(hx,x);
	ix = hx&0x7fffffff;
	if(ix<=0x3f490fd8)   /* |x| ~<= pi/4 , no need for reduction */
	    {y[0] = x; y[1] = 0; return 0;}
	if(ix<0x4016cbe4) {  /* |x| < 3pi/4, special case with n=+-1 */
	    /* 17+17+24 bit pi has sufficient precision and best efficiency */
	    if(hx>0) { 
	        z = x - pio2_1;
            if((ix&0xfffe0000U)!=0x3fc80000) { /* 17+24 bit pi OK */
                y[0] = z - pio2_1t;
                y[1] = (z-y[0])-pio2_1t;
            } else {        /* near pi/2, use 17+17+24 bit pi */
                z -= pio2_2;
                y[0] = z - pio2_2t;
                y[1] = (z-y[0])-pio2_2t;
            }
            return 1;
	    } else {	/* negative x */
	        z = x + pio2_1;
            if((ix&0xfffe0000U)!=0x3fc80000) { /* 17+24 bit pi OK */
                y[0] = z + pio2_1t;
                y[1] = (z-y[0])+pio2_1t;
            } else {        /* near pi/2, use 17+17+24 bit pi */
                z += pio2_2;
                y[0] = z + pio2_2t;
                y[1] = (z-y[0])+pio2_2t;
            }
            return -1;
	    }
	}
	if(ix<=0x43490f80) { /* |x| ~<= 2^7*(pi/2), medium size */
	    t  = fabsf(x);
	    n  = (__int32_t) (t*invpio2+half);
	    fn = (float)n;
	    r  = t-fn*pio2_1;
	    w  = fn*pio2_1t;	/* 1st round good to 40 bit */
	    {
	        __uint32_t high;
	        j  = ix>>23;
	        y[0] = r-w; 
		GET_FLOAT_WORD(high,y[0]);
	        i = j-((high>>23)&0xff);
	        if(i>8) {  /* 2nd iteration needed, good to 57 */
		    t  = r;
		    w  = fn*pio2_2;	
		    r  = t-w;
		    w  = fn*pio2_2t-((t-r)-w);	
		    y[0] = r-w;
		    GET_FLOAT_WORD(high,y[0]);
		    i = j-((high>>23)&0xff);
		    if(i>25)  {	/* 3rd iteration need, 74 bits acc */
		    	t  = r;	/* will cover all possible cases */
		    	w  = fn*pio2_3;	
		    	r  = t-w;
		    	w  = fn*pio2_3t-((t-r)-w);	
		    	y[0] = r-w;
		    }
		}
	    }
	    y[1] = (r-y[0])-w;
	    if(hx<0) 	{y[0] = -y[0]; y[1] = -y[1]; return -n;}
	    else	 return n;
	}
    /* 
     * all other (large) arguments
     */
	if(!FLT_UWORD_IS_FINITE(ix)) {
	    y[0]=y[1]=x-x; return 0;
	}
    /* set z = scalbn(|x|,ilogb(x)-7) */
	e0 	= (ix>>23)-134;	/* e0 = ilogb(z)-7; */
	SET_FLOAT_WORD(z, ix - (e0<<23));
	for(i=0;i<2;i++) {
		tx[i] = (float)((__int32_t)(z));
		z     = (z-tx[i])*two8;
	}
	tx[2] = z;
    for (nx = 3; nx>1; --nx) /* skip zero term */
    {
        if (tx[nx-1]!=zero) break;
    }
	n  =  __kernel_rem_pio2f(tx,y,e0,nx);
	if(hx<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
	return n;
}
