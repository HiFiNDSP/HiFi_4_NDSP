/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs ("Cadence    */
/* Libraries") are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2018 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
  NatureDSP Signal Processing Library. IIR part
    Bi-quad Real Block IIR, floating point, Direct Form II
    C code optimized for HiFi4
  IntegrIT, 2006-2018
*/

/*-------------------------------------------------------------------------
  Bi-quad Real Block IIR
  Computes a real IIR filter (cascaded IIR direct form I or II using 5 
  coefficients per bi-quad + gain term). Real input data are stored
  in vector x. The filter output result is stored in vector r. The filter 
  calculates N output samples using SOS and G matrices.
  NOTE:
  1. Bi-quad coefficients may be derived from standard SOS and G matrices 
  generated by MATLAB. However, typically biquad stages have big peaks 
  in their step response which may cause undesirable overflows at the 
  intermediate outputs. To avoid that the additional scale factors 
  coef_g[M] may be applied. These per-section scale factors may require 
  some tuning to find a compromise between quantization noise and possible 
  overflows. Output of the last section is directed to an additional 
  multiplier, with the gain factor being a power of two, either negative 
  or non-negative. It is specified through the total gain shift amount 
  parameter gain of each filter initialization function.
  2. 16x16 filters may suffer more from accumulation of the roundoff errors,
  so filters should be properly designed to match noise requirements
  3. Due to the performance reasons, IIR biquad filters may introduce 
  additional algorithmic delay of several sampless. Amount of that delay
  might be requested by the  xxx_groupDelay API. For sensitive applications
  all the filters have delayless implementations (with  _nd  suffix in the name).
  Formally, the xxx_groupDelay APIs is also implemented for that kind of filters,
  but return zero.
  
  Precision: 
  16x16         16-bit data, 16-bit coefficients, 16-bit intermediate 
                stage outputs (DF1, DF1 stereo, DF II form)
  32x16         32-bit data, 16-bit coefficients, 32-bit intermediate 
                (DF1, DF1 stereo, DF II form) stage outputs
  32x32         32-bit data, 32-bit coefficients, 32-bit intermediate 
                (DF I, DF1 stereo,  DF II form) stage outputs 
  f             floating point (DF I, DF1 stereo, DF II and DF IIt)

  Input:
  N             length of input sample block
  M             number of bi-quad sections
  S             1 for mono, 2 for stereo API
  s[]           scratch memory area (for fixed-point functions only). 
                Minimum number of bytes depends on selected filter structure 
                and precision:
                  BQRIIR16X16_DF1_SCRATCH_SIZE( N,  M ), or
                  BQRIIR16X16_DF2_SCRATCH_SIZE( N,  M ), or
                  BQRIIR32X16_DF1_SCRATCH_SIZE( N,  M ), or
                  BQRIIR32X16_DF2_SCRATCH_SIZE( N,  M ), or
                  BQRIIR32X32_DF1_SCRATCH_SIZE( N,  M ), or
                  BQRIIR32X32_DF2_SCRATCH_SIZE( N,  M ),
                  STEREO_BQRIIR16X16_DF1_SCRATCH_SIZE( N, M ) , or
                  STEREO_BQRIIR32X32_DF1_SCRATCH_SIZE( N, M ) , or
                  STEREO_BQRIIRF_DF1_SCRATCH_SIZE    ( N, M )
                 If a particular macro returns zero, then the corresponding
                 IIR doesn't require a scratch area and parameter s may 
                 hold zero.
  coef_sos[M*5]  filter coefficients stored in blocks of 5 numbers: 
                 b0 b1 b2 a1 a2. 
                 For fixed-point funcions, fixed point format of filter 
                 coefficients is Q1.14 for 16x16 and 32x16, or Q1.30 for 32x32 
  coef_sosl[M*5] filter coefficients for the left channel (stereo filters only)
  coef_sosr[M*5] filter coefficients for the left channel (stereo filters only)
  coef_g[M]      scale factor for each section, Q15 (for fixed-point 
                 functions only)
  coef_gl[M]     scale factor for the left channel (stereo filters only)
  coef_gr[M]     scale factor for the right channel (stereo filters only)
  gain           total gain shift amount applied to output signal of the
                 last section, -48..15
  gainl          total gain shift amount  for the left channel (stereo filters 
                 only)
  gainr          total gain shift amount for the right channel (stereo filters 
                 only)

  x[N*S]         input samples, Q15, Q31 or floating point. Stereo samples 
                 go in interleaved order (left, right)
  Output:
  r[N*S]         output data, Q15, Q31 or floating point. Stereo samples go 
                 in interleaved order (left, right) 

  Restriction:
  x,r,s,coef_g,coef_sos should not overlap
  N   - must be a multiple of 2
  s[] - whenever supplied must be aligned on an 8-bytes boundary
-------------------------------------------------------------------------*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_iir.h"
/* Common utility and macros declarations. */
#include "common.h"
#include "bqriirf_df2.h"
#include "common_fpu.h"

#if (HAVE_VFPU==0 && HAVE_FPU==0)
DISCARD_FUN(void,bqriirf_df2,( bqriirf_df2_handle_t _bqriir,
                      float32_t * restrict       z,
                const float32_t *            x, int N))
#elif (HAVE_VFPU)

// Process data. The filter instance is identified with a handle returned by
// the initializing function.
void bqriirf_df2( bqriirf_df2_handle_t _bqriir,
                  float32_t * restrict       z,
            const float32_t *                x, int N)
{
    bqriirf_df2_t *state;
    const ae_int32x2 * restrict pX;
          ae_int32x2 * restrict pZ;
    const xtfloatx2  * restrict pa1,
                     * restrict pa2,
                     * restrict pb0,
                     * restrict pb1,
                     * restrict pb2;
    const xtfloatx2  * restrict pDrd;
          xtfloatx2  * restrict pDwr;
    xtfloatx2 dx00,dy00,dy10,dy20,t0;
    xtfloatx2 dx01,dy01,dy11,dy21,t1;
    xtfloatx2 a10,a20,b00,b10,b20;
    xtfloatx2 a11,a21,b01,b11,b21;
    int n,m;
    int M;

    NASSERT(_bqriir);
    if (N<2) return;
    NASSERT(N%2==0);
    NASSERT(x);
    NASSERT(z);
    state=(bqriirf_df2_t*)(_bqriir);
    NASSERT(state);
    NASSERT(state->st);
    NASSERT(state->cf);
    M=state->M;
    NASSERT(M>0);
    
    /* Initialize pointers */
    pX  =(const ae_int32x2*)x;
    pb0 =(const xtfloatx2*)(state->cf);
    pDrd=(const xtfloatx2*)(state->st);
    pDwr=(      xtfloatx2*)(state->st);
    
    /* Process samples by 4 sections */
    for (m = 0; m < (M>>2); m++)
    {
        pZ=(ae_int32x2 *)z;
        /* load delay lines */
        XT_LSX2IP(dx00,pDrd,2*sizeof(float32_t));
        XT_LSX2IP(dx01,pDrd,2*sizeof(float32_t));
        XT_LSX2IP(dy00,pDrd,2*sizeof(float32_t));
        XT_LSX2IP(dy01,pDrd,2*sizeof(float32_t));
        XT_LSX2IP(dy10,pDrd,2*sizeof(float32_t));
        XT_LSX2IP(dy11,pDrd,2*sizeof(float32_t));
        /* load part of the filter coefficients */
        b10 = XT_LSX2I(pb0, 4*sizeof(float32_t));
        b11 = XT_LSX2I(pb0, 6*sizeof(float32_t));
        /* processing loop  */
        for (n = 0; n < N; n++)
        {
            ae_int32x2 tmp;
            /* load input sample */
            AE_L32_IP(tmp,castxcc(ae_int32,pX),sizeof(float32_t)); t0=XT_AE_MOVXTFLOATX2_FROMINT32X2(tmp);
            /* load filter coefficients */
            b00 = XT_LSX2I(pb0, 0*sizeof(float32_t));
            b01 = XT_LSX2I(pb0, 2*sizeof(float32_t));
            b20 = XT_LSX2I(pb0, 8*sizeof(float32_t));
            b21 = XT_LSX2I(pb0,10*sizeof(float32_t));
            a10 = XT_LSX2I(pb0,12*sizeof(float32_t));
            a11 = XT_LSX2I(pb0,14*sizeof(float32_t));
            a20 = XT_LSX2X(pb0,16*sizeof(float32_t));
            a21 = XT_LSX2X(pb0,18*sizeof(float32_t));
            /* shift delay lines */
            dy20=dy10;
            dy10=dy00;
            dy00=XT_SEL32_HH_SX2(t0,dx00);
            dy21=dy11;
            dy11=dy01;
            dy01=XT_SEL32_LH_SX2(dx00,dx01);
            /* perform multiplications for 4 biquads */
            t0=XT_MUL_SX2(a20,dy20);
            XT_MADDN_SX2(t0,a10,dy10);
            dy00=dy00-t0;
            dx00=XT_MUL_SX2 (b20,dy20);
            XT_MADDN_SX2(dx00,b10,dy10);
            XT_MADDN_SX2(dx00,b00,dy00);
            
            t1=XT_MUL_SX2(a21,dy21);
            XT_MADDN_SX2(t1,a11,dy11);
            dy01=dy01-t1;
            dx01=XT_MUL_SX2 (b21,dy21);
            XT_MADDN_SX2(dx01,b11,dy11);
            XT_MADDN_SX2(dx01,b01,dy01);
            /* save computed sample */
            tmp=XT_AE_MOVINT32X2_FROMXTFLOATX2(dx01); AE_S32_L_IP(tmp,castxcc(ae_int32,pZ),sizeof(float32_t));
        }
        /* jump to the next 4 sections */
        pb0 += 10;
        /* update delay lines */
        XT_SSX2IP(dx00,pDwr,2*sizeof(float32_t));
        XT_SSX2IP(dx01,pDwr,2*sizeof(float32_t));
        XT_SSX2IP(dy00,pDwr,2*sizeof(float32_t));
        XT_SSX2IP(dy01,pDwr,2*sizeof(float32_t));
        XT_SSX2IP(dy10,pDwr,2*sizeof(float32_t));
        XT_SSX2IP(dy11,pDwr,2*sizeof(float32_t));
        pX=(const ae_int32x2*)z;/* switch pointer to the input data to the pointer to the output */
    }
    /* Process last M%4 sections */
    pb1 =( const xtfloatx2*)((float32_t *)pb0+(M&3));
    pb2 =( const xtfloatx2*)((float32_t *)pb1+(M&3));
    pa1 =( const xtfloatx2*)((float32_t *)pb2+(M&3));
    pa2 =( const xtfloatx2*)((float32_t *)pa1+(M&3));

    for(m = (M & ~3); m < M; m++)
    {
        xtfloat dx0,dy0,dy1,dy2;
        xtfloat a1,a2,b0,b1,b2;
        pZ=(ae_int32x2 *)z;
        /* load delay lines and coefficients */
        XT_LSIP(dx0,castxcc(xtfloat,pDrd),sizeof(float32_t)); 
        XT_LSIP(dy0,castxcc(xtfloat,pDrd),sizeof(float32_t)); 
        XT_LSIP(dy1,castxcc(xtfloat,pDrd),sizeof(float32_t)); 
        XT_LSIP(b0,castxcc(xtfloat,pb0),sizeof(float32_t));
        XT_LSIP(b1,castxcc(xtfloat,pb1),sizeof(float32_t));
        XT_LSIP(b2,castxcc(xtfloat,pb2),sizeof(float32_t));
        XT_LSIP(a1,castxcc(xtfloat,pa1),sizeof(float32_t)); a1=XT_NEG_S(a1);
        XT_LSIP(a2,castxcc(xtfloat,pa2),sizeof(float32_t));
        /* processing loop  */
        for (n = 0; n < N; n++)
        {
            XT_LSIP(dx0,castxcc(xtfloat,pX),sizeof(x[0]));
            dy2=dy1;
            dy1=dy0;
            dy0=dx0;
            XT_MSUBN_S(dy0,a2,dy2);
            XT_MADDN_S(dy0,a1,dy1);
            dx0=XT_MUL_S (b2,dy2);
            XT_MADDN_S(dx0,b1,dy1);
            XT_MADDN_S(dx0,b0,dy0);
            XT_SSIP(dx0,castxcc(xtfloat,pZ),sizeof(x[0]));
        }
        XT_SSIP(dx0,castxcc(xtfloat,pDwr),sizeof(float32_t));
        XT_SSIP(dy0,castxcc(xtfloat,pDwr),sizeof(float32_t));
        XT_SSIP(dy1,castxcc(xtfloat,pDwr),sizeof(float32_t));
        pX=(const ae_int32x2*)z;/* switch pointer to the input data to the pointer to the output */
    }
    /* final scaling */
    {
        xtfloatx2 ft;
        int32_t s;
        s=state->gain;
        s=((s+127)&255)<<23;
        ft=XT_AE_MOVXTFLOATX2_FROMF32X2(AE_MOVDA32X2(s,s));
        pZ=(ae_int32x2 *)z;

        if (((uintptr_t)z)&(sizeof(ae_int32x2)-1))
        {
            xtfloat t0, ft0;
            ft0 = XT_WFR(s);
            /* output pointer is not aligned by 8-byte boundary */
            XT_LSIP( t0, castxcc(xtfloat,pX), sizeof(xtfloat) );
            t0 = XT_MUL_S(t0, ft0);
            XT_SSIP( t0, castxcc(xtfloat,pZ), sizeof(xtfloat) );
            t0 = XT_LSX( (xtfloat *)pX, (N-2)*sizeof(xtfloat) );
            t0 = XT_MUL_S(t0, ft0);
            XT_SSX( t0, (xtfloat *)pZ, (N-2)*sizeof(xtfloat) );
            N -= 2;
        }
        for (n = 0; n < (N>>1); n++)
        {
            XT_LSX2IP(t0, castxcc(xtfloatx2,pX), sizeof(xtfloatx2));
            t0 = t0*ft;
            XT_SSX2IP(t0, castxcc(xtfloatx2,pZ), sizeof(xtfloatx2));
        }
    }
}/* bqriirf_df2_process() */
#else 
// for scalar FPU
void bqriirf_df2( bqriirf_df2_handle_t _bqriir,
                  float32_t * restrict       z,
            const float32_t *                x, int N)
{
#define VLEN 4  /* vector length */
    bqriirf_df2_t *state;
    const float32_t *cf;
    const xtfloat * restrict pDrd;
          xtfloat * restrict pDwr;
          xtfloat * pZ;
    const xtfloat * pX;
    const xtfloat * restrict pXr;
          xtfloat * restrict pZr;
    float32_t scale;
    int n,m;
    int32_t s;
    int M,M1;
    NASSERT(_bqriir);
    state=(bqriirf_df2_t*)(_bqriir);
    if(N<=0) return;
    NASSERT(N%2==0);
    NASSERT(x);
    NASSERT(z);
    NASSERT(state);
    NASSERT(state->st);
    NASSERT(state->cf);
    M=state->M;
    M1 = (M)&~(VLEN-1);  /* truncate M to multiple of VLEN */
    cf=state->cf;
    pDrd=(const xtfloat*)state->st;
    pDwr=(      xtfloat*)state->st;
    s=state->gain;
    s=((s+127)&255)<<23;
    scale=XT_WFR(s);

    for(m=0; m<M; m+=VLEN) 
    {
        int p,P = XT_MIN(VLEN,M-m);
        for(p=0;p<P;p++)
        {
            int noExraDelay= P<VLEN || (p==VLEN-1);
            xtfloat dx0,dy0,dy1,dy2;
            xtfloat a1 ,a2 ,b0 ,b1 ,b2;
            /* load delay lines and coefficients */
            XT_LSIP(dx0,pDrd,sizeof(xtfloat));
            XT_LSIP(dy0,pDrd,sizeof(xtfloat));
            XT_LSIP(dy1,pDrd,sizeof(xtfloat));
            b0=cf[0*P+p+m*5]; 
            b1=cf[1*P+p+m*5]; 
            b2=cf[2*P+p+m*5]; 
            a1=cf[3*P+p+m*5]; 
            a2=cf[4*P+p+m*5]; 
            pZ=(      xtfloat*)z;
            pX=(const xtfloat*)x;
            if (noExraDelay)
            {
                for (n=0;n<N;n++)
                {
                    XT_LSIP(dx0,pX,sizeof(xtfloat));
                    dy2=dy1;
                    dy1=dy0;
                    dy0=dx0;
                    XT_MSUB_S(dy0,a2,dy2);
                    XT_MSUB_S(dy0,a1,dy1);
                    dx0 =XT_MUL_S(b2,dy2);
                    XT_MADD_S(dx0,b1,dy1);
                    XT_MADD_S(dx0,b0,dy0);
                    XT_SSIP(dx0,pZ,sizeof(xtfloat));
                }
            }
            else
            {

                for (n=0;n<N;n+=2)
                {
                    xtfloat old_dx0=dx0,dx1;
                    XT_LSIP(dx0,pX,sizeof(xtfloat));
                    XT_LSIP(dx1,pX,sizeof(xtfloat));
                    XT_SSIP(old_dx0,pZ,sizeof(xtfloat));
                    dy2=dy1;
                    dy1=dy0;
                    dy0=dx0;
                    XT_MSUB_S(dy0,a2,dy2);
                    XT_MSUB_S(dy0,a1,dy1);
                    dx0 =XT_MUL_S(b2,dy2);
                    XT_MADD_S(dx0,b1,dy1);
                    XT_MADD_S(dx0,b0,dy0);
                    XT_SSIP(dx0,pZ,sizeof(xtfloat));

                    dy2=dy1;
                    dy1=dy0;
                    dy0=dx1;
                    XT_MSUB_S(dy0,a2,dy2);
                    XT_MSUB_S(dy0,a1,dy1);
                    dx0 =XT_MUL_S(b2,dy2);
                    XT_MADD_S(dx0,b1,dy1);
                    XT_MADD_S(dx0,b0,dy0);
                }

            }
            /* save delay lines  */
            XT_SSIP(dx0,pDwr,sizeof(xtfloat));
            XT_SSIP(dy0,pDwr,sizeof(xtfloat));
            XT_SSIP(dy1,pDwr,sizeof(xtfloat));
            x=z;    /* switch pointer to the input data to the pointer to the output */
        }
    }
    /* final scaling */
    __Pragma("no_reorder")
    {
        pZr=(      xtfloat*)z;
        pXr=(const xtfloat*)x;
        for (n=0; n<(N>>1); n++)
        {
            xtfloat x0,x1;
            XT_LSIP(x0,pXr,sizeof(xtfloat));
            XT_LSIP(x1,pXr,sizeof(xtfloat));
            XT_SSIP(XT_MUL_S(x0,scale),pZr,sizeof(xtfloat));
            XT_SSIP(XT_MUL_S(x1,scale),pZr,sizeof(xtfloat));
        }
    }
#undef VLEN
} 
#endif
