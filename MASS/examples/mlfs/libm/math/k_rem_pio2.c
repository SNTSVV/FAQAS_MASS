/*!
 * -------------------------------------------------------------------------------------------------
 * \file k_rem_pio2.c
 * \par ESA Contract Number
 *      4000117691/16/NL/FE
 * \brief Implementation file for kernel range reduction.
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
 * -----------|--------------|-----------------------------------------------------
 * 2017-06-07 | F. Schriever | Initialise arrays
 * 2017-06-09 | F. Schriever | Replace possibly not ending while-loop with for-loop
 * 2017-06-21 | F. Schriever | Remove input variable prec and it's usage
 * 2017-07-31 | F. Schriever | Move array ipio2 from __ieee754_rem_pio2 here, and remove ipio2 as input
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* @(#)k_rem_pio2.c 5.1 93/09/24 */
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
 * __kernel_rem_pio2(x,y,e0,nx,ipio2)
 * double x[],y[]; int e0,nx; int ipio2[];
 * 
 * __kernel_rem_pio2 return the last three digits of N with 
 *		y = x - N*pi/2
 * so that |y| < pi/2.
 *
 * The method is to compute the integer (mod 8) and fraction parts of 
 * (2/pi)*x without doing the full multiplication. In general we
 * skip the part of the product that are known to be a huge integer (
 * more accurately, = 0 mod 8 ). Thus the number of operations are
 * independent of the exponent of the input.
 *
 * (2/pi) is represented by an array of 24-bit integers in ipio2[].
 *
 * Input parameters:
 * 	x[]	The input value (must be positive) is broken into nx 
 *		pieces of 24-bit integers in double precision format.
 *		x[i] will be the i-th 24 bit of x. The scaled exponent 
 *		of x[0] is given in input parameter e0 (i.e., x[0]*2^e0 
 *		match x's up to 24 bits.
 *
 *		Example of breaking a double positive z into x[0]+x[1]+x[2]:
 *			e0 = ilogb(z)-23
 *			z  = scalbn(z,-e0)
 *		for i = 0,1,2
 *			x[i] = floor(z)
 *			z    = (z-x[i])*2**24
 *
 *
 *	y[]	output result in an array of double precision numbers.
 *		The dimension of y[] is 2 which results in 53-bit precision.
 *		The actual value is the sum of them.
 *
 *	e0	The exponent of x[0]
 *
 *	nx	dimension of x[]
 *
 * External function:
 *	double scalbn(), floor();
 *
 *
 * Here is the description of some local variables:
 *
 *  ipio2[]
 *      integer array, contains the (24*i)-th to (24*i+23)-th 
 *      bit of 2/pi after binary point. The corresponding 
 *      floating value is
 *
 *          ipio2[i] * 2^(-24(i+1)).
 *
 * 	jk	jk+1 is the initial number of terms of ipio2[] needed
 *		in the computation. The recommended value is 2,3,4,
 *		6 for single, double, extended,and quad.
 *
 * 	jz	local integer variable indicating the number of 
 *		terms of ipio2[] used. 
 *
 *	jx	nx - 1
 *
 *	jv	index for pointing to the suitable ipio2[] for the
 *		computation. In general, we want
 *			( 2^e0*x[0] * ipio2[jv-1]*2^(-24jv) )/8
 *		is an integer. Thus
 *			e0-3-24*jv >= 0 or (e0-3)/24 >= jv
 *		Hence jv = max(0,(e0-3)/24).
 *
 *	jp	jp+1 is the number of terms in PIo2[] needed, jp = jk.
 *
 * 	q[]	double array with integral value, representing the
 *		24-bits chunk of the product of x and 2/pi.
 *
 *	q0	the corresponding exponent of q[0]. Note that the
 *		exponent for q[i] would be q0-24*i.
 *
 *	PIo2[]	double precision array, obtained by cutting pi/2
 *		into 24 bits chunks. 
 *
 *	f[]	ipio2[] in floating point 
 *
 *	iq[]	integer array by breaking up q[] in 24-bits chunk.
 *
 *	fq[]	final product of x*(2/pi) in fq[0],..,fq[jk]
 *
 *	ih	integer. If >0 it indicates q[] is >= 0.5, hence
 *		it also indicates the *sign* of the result.
 *
 */


/*
 * Constants:
 * The hexadecimal values are the intended ones for the following 
 * constants. The decimal values may be used, provided that the 
 * compiler will convert from decimal to binary accurately enough 
 * to produce the hexadecimal values shown.
 */

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
 */
#ifdef __STDC__
static const __int32_t ipio2[] = {
#else
static __int32_t ipio2[] = {
#endif
0xA2F983, 0x6E4E44, 0x1529FC, 0x2757D1, 0xF534DD, 0xC0DB62,
0x95993C, 0x439041, 0xFE5163, 0xABDEBB, 0xC561B7, 0x246E3A,
0x424DD2, 0xE00649, 0x2EEA09, 0xD1921C, 0xFE1DEB, 0x1CB129,
0xA73EE8, 0x8235F5, 0x2EBB44, 0x84E99C, 0x7026B4, 0x5F7E41,
0x3991D6, 0x398353, 0x39F49C, 0x845F8B, 0xBDF928, 0x3B1FF8,
0x97FFDE, 0x05980F, 0xEF2F11, 0x8B5A0A, 0x6D1F6D, 0x367ECF,
0x27CB09, 0xB74F46, 0x3F669E, 0x5FEA2D, 0x7527BA, 0xC7EBE5,
0xF17B3D, 0x0739F7, 0x8A5292, 0xEA6BFB, 0x5FB11F, 0x8D5D08,
0x560330, 0x46FC7B, 0x6BABF0, 0xCFBC20, 0x9AF436, 0x1DA9E3,
0x91615E, 0xE61B08, 0x659985, 0x5F14A0, 0x68408D, 0xFFD880,
0x4D7327, 0x310606, 0x1556CA, 0x73A8C9, 0x60E27B, 0xC08C6B,
};

#ifdef __STDC__
static const double PIo2[] = {
#else
static double PIo2[] = {
#endif
  1.57079625129699707031e+00, /* 0x3FF921FB, 0x40000000 */
  7.54978941586159635335e-08, /* 0x3E74442D, 0x00000000 */
  5.39030252995776476554e-15, /* 0x3CF84698, 0x80000000 */
  3.28200341580791294123e-22, /* 0x3B78CC51, 0x60000000 */
  1.27065575308067607349e-29, /* 0x39F01B83, 0x80000000 */
  1.22933308981111328932e-36, /* 0x387A2520, 0x40000000 */
  2.73370053816464559624e-44, /* 0x36E38222, 0x80000000 */
  2.16741683877804819444e-51, /* 0x3569F31D, 0x00000000 */
};

#ifdef __STDC__
static const double			
#else
static double			
#endif
zero   = 0.0,
one    = 1.0,
two24   =  1.67772160000000000000e+07, /* 0x41700000, 0x00000000 */
twon24  =  5.96046447753906250000e-08; /* 0x3E700000, 0x00000000 */

#ifdef __STDC__
	__int32_t __kernel_rem_pio2(double *x, double *y, __int32_t e0, __int32_t nx)
#else
	__int32_t __kernel_rem_pio2(x,y,e0,nx)
	double x[], y[]; __int32_t e0,nx;
#endif
{
	__int32_t jz,jx,jv,jp,jk,carry,n,i,j,k,m,q0,ih;
	double z,fw;
	bool recompute;

    /* Init arrays. */
    __int32_t iq[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    double f[20]  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
           fq[20] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
           q[20]  = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

    /* initialize jk*/
	jk = 4;
	jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
	jx =  nx-1;
	jv = (e0-3)/24; if(jv<0) jv=0;
	q0 =  e0-24*(jv+1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv-jx; m = jx+jk;
	for(i=0;i<=m;i++,j++) f[i] = (j<0)? zero : (double) ipio2[j];

    /* compute q[0],q[1],...q[jk] */
	for (i=0;i<=jk;i++) {
	    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*f[jx+i-j];
	    q[i] = fw;
	}

	jz = jk;
    do {
        recompute = false;
        /* distill q[] into iq[] reversingly */
        for(i=0,j=jz,z=q[jz];j>0;i++,j--) {
            fw    =  (double)((__int32_t)(twon24* z));
            iq[i] =  (__int32_t)(z-two24*fw);
            z     =  q[j-1]+fw;
        }

        /* compute n */
        z  = scalbn(z,(int)q0);		/* actual value of z */
        z -= 8.0*floor(z*0.125);		/* trim off integer >= 8 */
        n  = (__int32_t) z;
        z -= (double)n;
        ih = 0;
        if(q0>0) {	/* need iq[jz-1] to determine n */
            i  = (iq[jz-1]>>(24-q0)); n += i;
            iq[jz-1] -= i<<(24-q0);
            ih = iq[jz-1]>>(23-q0);
        } 
        else if(q0==0) ih = iq[jz-1]>>23;
        else if(z>=0.5) ih=2;

        if(ih>0) {	/* q > 0.5 */
            n += 1; carry = 0;
            for(i=0;i<jz ;i++) {	/* compute 1-q */
                j = iq[i];
                if(carry==0) {
                    if(j!=0) {
                        carry = 1; iq[i] = 0x1000000- j;
                    }
                } else  iq[i] = 0xffffff - j;
            }
            if(q0>0) {		/* rare case: chance is 1 in 12 */
                switch(q0) {
                default:
                case 1:
                   iq[jz-1] &= 0x7fffff; break;
                case 2:
                   iq[jz-1] &= 0x3fffff; break;
                }
            }
            if(ih==2) {
                z = one - z;
                if(carry!=0) z -= scalbn(one,(int)q0);
            }
        }

        /* check if recomputation is needed */
        if(z==zero) {
            j = 0;
            for (i=jz-1;i>=jk;i--) j |= iq[i];
            if(j==0) { /* need recomputation */
                for(k=1;iq[jk-k]==0;k++);   /* k = no. of terms needed */
    
                for(i=jz+1;i<=jz+k;i++) {   /* add q[jz+1] to q[jz+k] */
                    f[jx+i] = (double) ipio2[jv+i];
                    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*f[jx+i-j];
                    q[i] = fw;
                }
                jz += k;
                recompute = true;
            }
        }
    } while (recompute);

    /* chop off zero terms */
	if(z==0.0) {
	    q0 -= 24;
        for (jz -= 1; jz>=0; --jz)
        {
            if (iq[jz]!=0) break;
            q0 -= 24;
        }
	} else { /* break z into 24-bit if necessary */
	    z = scalbn(z,-(int)q0);
	    if(z>=two24) { 
		fw = (double)((__int32_t)(twon24*z));
		iq[jz] = (__int32_t)(z-two24*fw);
		jz += 1; q0 += 24;
		iq[jz] = (__int32_t) fw;
	    } else iq[jz] = (__int32_t) z ;
	}

    /* convert integer "bit" chunk to floating-point value */
	fw = scalbn(one,(int)q0);
	for(i=jz;i>=0;i--) {
	    q[i] = fw*(double)iq[i]; fw*=twon24;
	}

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
	for(i=jz;i>=0;i--) {
	    for(fw=0.0,k=0;k<=jp&&k<=jz-i;k++) fw += PIo2[k]*q[i+k];
	    fq[jz-i] = fw;
	}

    /* compress fq[] into y[] */
    fw = 0.0;
    for (i=jz;i>=0;i--) fw += fq[i]; 
    y[0] = (ih==0)? fw: -fw;
    fw = fq[0]-fw;
    for (i=1;i<=jz;i++) fw += fq[i];
    y[1] = (ih==0)? fw: -fw;

    return n&7;
}

#endif /* defined(_DOUBLE_IS_32BITS) */
