/*!
 * -------------------------------------------------------------------------------------------------
 * \file kf_rem_pio2.c
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
 * 2017-07-31 | F. Schriever | Fix shifting error introduced when originally converting from the double precision version of this function.
 * 2017-07-31 | F. Schriever | Move array ipio2 from __ieee754_rem_pio2 here, and remove ipio2 as input
 * 
 * \copyright Modified work Copyright European Space Agency, 2017.
 */

/* kf_rem_pio2.c -- float version of k_rem_pio2.c
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

/* In the float version, the input parameter x contains 8 bit
   integers, not 24 bit integers.  113 bit precision is not supported.  */

/*
 * Table of constants for 2/pi, 396 Hex digits (476 decimal) of 2/pi
 */
#ifdef __STDC__
static const __int32_t ipio2[] = {
#else
static __int32_t ipio2[] = {
#endif
0xA2, 0xF9, 0x83, 0x6E, 0x4E, 0x44, 0x15, 0x29, 0xFC,
0x27, 0x57, 0xD1, 0xF5, 0x34, 0xDD, 0xC0, 0xDB, 0x62,
0x95, 0x99, 0x3C, 0x43, 0x90, 0x41, 0xFE, 0x51, 0x63,
0xAB, 0xDE, 0xBB, 0xC5, 0x61, 0xB7, 0x24, 0x6E, 0x3A,
0x42, 0x4D, 0xD2, 0xE0, 0x06, 0x49, 0x2E, 0xEA, 0x09,
0xD1, 0x92, 0x1C, 0xFE, 0x1D, 0xEB, 0x1C, 0xB1, 0x29,
0xA7, 0x3E, 0xE8, 0x82, 0x35, 0xF5, 0x2E, 0xBB, 0x44,
0x84, 0xE9, 0x9C, 0x70, 0x26, 0xB4, 0x5F, 0x7E, 0x41,
0x39, 0x91, 0xD6, 0x39, 0x83, 0x53, 0x39, 0xF4, 0x9C,
0x84, 0x5F, 0x8B, 0xBD, 0xF9, 0x28, 0x3B, 0x1F, 0xF8,
0x97, 0xFF, 0xDE, 0x05, 0x98, 0x0F, 0xEF, 0x2F, 0x11,
0x8B, 0x5A, 0x0A, 0x6D, 0x1F, 0x6D, 0x36, 0x7E, 0xCF,
0x27, 0xCB, 0x09, 0xB7, 0x4F, 0x46, 0x3F, 0x66, 0x9E,
0x5F, 0xEA, 0x2D, 0x75, 0x27, 0xBA, 0xC7, 0xEB, 0xE5,
0xF1, 0x7B, 0x3D, 0x07, 0x39, 0xF7, 0x8A, 0x52, 0x92,
0xEA, 0x6B, 0xFB, 0x5F, 0xB1, 0x1F, 0x8D, 0x5D, 0x08,
0x56, 0x03, 0x30, 0x46, 0xFC, 0x7B, 0x6B, 0xAB, 0xF0,
0xCF, 0xBC, 0x20, 0x9A, 0xF4, 0x36, 0x1D, 0xA9, 0xE3,
0x91, 0x61, 0x5E, 0xE6, 0x1B, 0x08, 0x65, 0x99, 0x85,
0x5F, 0x14, 0xA0, 0x68, 0x40, 0x8D, 0xFF, 0xD8, 0x80,
0x4D, 0x73, 0x27, 0x31, 0x06, 0x06, 0x15, 0x56, 0xCA,
0x73, 0xA8, 0xC9, 0x60, 0xE2, 0x7B, 0xC0, 0x8C, 0x6B,
};

#ifdef __STDC__
static const float PIo2[] = {
#else
static float PIo2[] = {
#endif
  1.5703125000e+00f, /* 0x3fc90000 */
  4.5776367188e-04f, /* 0x39f00000 */
  2.5987625122e-05f, /* 0x37da0000 */
  7.5437128544e-08f, /* 0x33a20000 */
  6.0026650317e-11f, /* 0x2e840000 */
  7.3896444519e-13f, /* 0x2b500000 */
  5.3845816694e-15f, /* 0x27c20000 */
  5.6378512969e-18f, /* 0x22d00000 */
  8.3009228831e-20f, /* 0x1fc40000 */
  3.2756352257e-22f, /* 0x1bc60000 */
  6.3331015649e-25f, /* 0x17440000 */
};

#ifdef __STDC__
static const float			
#else
static float			
#endif
zero   = 0.0f,
one    = 1.0f,
two8   =  2.5600000000e+02f, /* 0x43800000 */
twon8  =  3.9062500000e-03f; /* 0x3b800000 */

#ifdef __STDC__
	__int32_t __kernel_rem_pio2f(float *x, float *y, __int32_t e0, __int32_t nx)
#else
	__int32_t __kernel_rem_pio2f(x,y,e0,nx)
	float x[], y[]; __int32_t e0,nx;
#endif
{
	__int32_t jz,jx,jv,jp,jk,carry,n,i,j,k,m,q0,ih;
	float z,fw;
	bool recompute;

    /* Init arrays. */
    __int32_t iq[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    float f[20]  = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
          fq[20] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
          q[20]  = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    /* initialize jk*/
	jk = 7;
	jp = jk;

    /* determine jx,jv,q0, note that 3>q0 */
	jx =  nx-1;
	jv = (e0-3)/8; if(jv<0) jv=0;
	q0 =  e0-8*(jv+1);

    /* set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk] */
	j = jv-jx; m = jx+jk;
	for(i=0;i<=m;i++,j++) f[i] = (j<0)? zero : (float) ipio2[j];

    /* compute q[0],q[1],...q[jk] */
	for (i=0;i<=jk;i++) {
	    for(j=0,fw=0.0f;j<=jx;j++) fw += x[j]*f[jx+i-j];
	    q[i] = fw;
	}

	jz = jk;
    do {
        recompute = false;
        /* distill q[] into iq[] reversingly */
        for(i=0,j=jz,z=q[jz];j>0;i++,j--) {
            fw    =  (float)((__int32_t)(twon8* z));
            iq[i] =  (__int32_t)(z-two8*fw);
            z     =  q[j-1]+fw;
        }

        /* compute n */
        z  = scalbnf(z,(int)q0);	/* actual value of z */
        z -= (float)8.0*floorf(z*(float)0.125);	/* trim off integer >= 8 */
        n  = (__int32_t) z;
        z -= (float)n;
        ih = 0;
        if(q0>0) {	/* need iq[jz-1] to determine n */
            i  = (iq[jz-1]>>(8-q0)); n += i;
            iq[jz-1] -= i<<(8-q0);
            ih = iq[jz-1]>>(7-q0);
        } 
        else if(q0==0) ih = iq[jz-1]>>7;
        else if(z>=(float)0.5) ih=2;

        if(ih>0) {	/* q > 0.5 */
            n += 1; carry = 0;
            for(i=0;i<jz ;i++) {	/* compute 1-q */
                j = iq[i];
                if(carry==0) {
                    if(j!=0) {
                        carry = 1; iq[i] = 0x100- j;
                    }
                } else  iq[i] = 0xff - j;
            }
            if(q0>0) {		/* rare case: chance is 1 in 12 */
                switch(q0) {
                default:
                case 1:
                   iq[jz-1] &= 0x7f; break;
                case 2:
                   iq[jz-1] &= 0x3f; break;
                }
            }
            if(ih==2) {
                z = one - z;
                if(carry!=0) z -= scalbnf(one,(int)q0);
            }
        }

        /* check if recomputation is needed */
        if(z==zero) {
            j = 0;
            for (i=jz-1;i>=jk;i--) j |= iq[i];
            if(j==0) { /* need recomputation */
                for(k=1;iq[jk-k]==0;k++);   /* k = no. of terms needed */
    
                for(i=jz+1;i<=jz+k;i++) {   /* add q[jz+1] to q[jz+k] */
                    f[jx+i] = (float) ipio2[jv+i];
                    for(j=0,fw=0.0f;j<=jx;j++) fw += x[j]*f[jx+i-j];
                    q[i] = fw;
                }
                jz += k;
                recompute = true;
            }
        }
    } while (recompute);

    /* chop off zero terms */
	if(z==(float)0.0) {
        q0 -= 8;
        for (jz -= 1; jz>=0; --jz)
        {
            if (iq[jz]!=0) break;
            q0 -= 8;
        }
	} else { /* break z into 8-bit if necessary */
	    z = scalbnf(z,-(int)q0);
	    if(z>=two8) { 
		fw = (float)((__int32_t)(twon8*z));
		iq[jz] = (__int32_t)(z-two8*fw);
		jz += 1; q0 += 8;
		iq[jz] = (__int32_t) fw;
	    } else iq[jz] = (__int32_t) z ;
	}

    /* convert integer "bit" chunk to floating-point value */
	fw = scalbnf(one,(int)q0);
	for(i=jz;i>=0;i--) {
	    q[i] = fw*(float)iq[i]; fw*=twon8;
	}

    /* compute PIo2[0,...,jp]*q[jz,...,0] */
	for(i=jz;i>=0;i--) {
	    for(fw=0.0f,k=0;k<=jp&&k<=jz-i;k++) fw += PIo2[k]*q[i+k];
	    fq[jz-i] = fw;
	}

    /* compress fq[] into y[] */
    fw = 0.0f;
    for (i=jz;i>=0;i--) fw += fq[i]; 
    y[0] = (ih==0)? fw: -fw; 
    fw = fq[0]-fw;
    for (i=1;i<=jz;i++) fw += fq[i];
    y[1] = (ih==0)? fw: -fw; 

    return n&7;
}
