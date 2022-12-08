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
    Real interpolating FIR Filter
    C code optimized for HiFi4
    IntegrIT, 2006-2018
 */

/* Cross-platform data type definitions. */
#include "NatureDSP_types.h"
/* Common helper macros. */
#include "common.h"
#include "fir_interpf_4x.h"
#include "common_fpu.h"

#if (HAVE_VFPU)

/*-------------------------------------------------------------------------
    4x interpolator:
    Input/output:
    delay[M*4] - circular delay line
    Input:
    p        - pointer to delay line
    x[N]     - input signal
    h[M*4]   - impulse response
    N        - number of output samples
    Output:
    z[N*4]     - output samples
    Restrictions:
    N>0, M>0
    N   - multiple of 8
    M   - multiple of 4
    delay should be aligned on 8 byte boundary

    Returns:
    updated pointer to delay line
-------------------------------------------------------------------------*/
float32_t * fir_interpf_4x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x, int M, int N)
{
        xtfloatx2 *restrict pZ;
        xtfloatx2 *restrict pX;
  const xtfloatx2 *restrict pHa;
  const xtfloatx2 *restrict pHb;
  const xtfloatx2 *restrict pHc;
  const xtfloatx2 *restrict pHd;
  const xtfloatx2 *restrict pDld;
        xtfloatx2 *restrict pDst;
  xtfloatx2 acc01a, acc01b, acc01c, acc01d;
  xtfloatx2 acc23a, acc23b, acc23c, acc23d;
  xtfloatx2 x_1, x01, x23;
  xtfloatx2 h0a, h0b, h0c, h0d;
  xtfloatx2 temp;
  xtfloat   s0, s1, s2, s3;
  ae_valign z_align;
  int n, m;

  NASSERT(x);
  NASSERT(z);
  NASSERT(N>0);
  NASSERT(M>0);

  /* set bounds of the delay line */
  WUR_AE_CBEGIN0((uintptr_t)(delay));
  WUR_AE_CEND0((uintptr_t)(delay + M));
  /* initialize pointers */
  pDst = (xtfloatx2 *)p;
  pX   = (xtfloatx2 *)x;
  pZ   = (xtfloatx2 *)z;
  z_align = AE_ZALIGN64();
  /* Process by 4 input samples (16 output samples) */
  __Pragma("loop_count min=1")
  for (n = 0; n < (N>>2); n++)
  {
      pHa = (const xtfloatx2 *)(h);
      pHb = (const xtfloatx2 *)((float32_t *)pHa+M);
      pHc = (const xtfloatx2 *)((float32_t *)pHb+M);
      pHd = (const xtfloatx2 *)((float32_t *)pHc+M);
      acc01a = acc01b = acc01c = acc01d = (xtfloatx2)(0.0f);
      acc23a = acc23b = acc23c = acc23d = (xtfloatx2)(0.0f);
      /* preload input samples */
      x01 = XT_LSX2I(pX, 0);
      x23 = XT_LSX2I(pX, 2*sizeof(float32_t));
      /* prepare for reverse loading of the delay line */
      pDld = pDst;
      AE_ADDCIRC32X2_XC(castxcc(ae_int32x2,pDld), -(int)sizeof(float32_t));

      /* kernel loop: compute by 1 tap for each sample */
      __Pragma("loop_count min=2,factor=2")
      for (m = 0; m < M; m++)
      {
          /* load filter coefficients */
          XT_LSIP(s0, castxcc(xtfloat,pHa), sizeof(float32_t));
          XT_LSIP(s1, castxcc(xtfloat,pHb), sizeof(float32_t));
          XT_LSIP(s2, castxcc(xtfloat,pHc), sizeof(float32_t));
          XT_LSIP(s3, castxcc(xtfloat,pHd), sizeof(float32_t));
          h0a = s0;
          h0b = s1;
          h0c = s2;
          h0d = s3;
          /* multiply */
          XT_MADD_SX2(acc01a, h0a, x01);
          XT_MADD_SX2(acc01b, h0b, x01);
          XT_MADD_SX2(acc01c, h0c, x01);
          XT_MADD_SX2(acc01d, h0d, x01);
          XT_MADD_SX2(acc23a, h0a, x23);
          XT_MADD_SX2(acc23b, h0b, x23);
          XT_MADD_SX2(acc23c, h0c, x23);
          XT_MADD_SX2(acc23d, h0d, x23);
          /* load sample from the delay line *
           * and shift whole line            */
          XT_LSXC(s0, castxcc(xtfloat,pDld), -(int)sizeof(float32_t));
          x_1 = s0;
          x23 = XT_SEL32_LH_SX2(x01, x23);
          x01 = XT_SEL32_LH_SX2(x_1, x01);
      }
      /* save computed samples */
      temp = XT_SEL32_HH_SX2(acc01a, acc01b);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_HH_SX2(acc01c, acc01d);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_LL_SX2(acc01a, acc01b);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_LL_SX2(acc01c, acc01d);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_HH_SX2(acc23a, acc23b);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_HH_SX2(acc23c, acc23d);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_LL_SX2(acc23a, acc23b);    XT_SASX2IP(temp, z_align, pZ);
      temp = XT_SEL32_LL_SX2(acc23c, acc23d);    XT_SASX2IP(temp, z_align, pZ);
      /* update the delay line */
      XT_LSX2IP(x01, pX  , 2*sizeof(float32_t));
      XT_LSX2IP(x23, pX  , 2*sizeof(float32_t));
      XT_SSX2XC(x01, pDst, 2*sizeof(float32_t));
      XT_SSX2XC(x23, pDst, 2*sizeof(float32_t));
  }
  XT_SASX2POSFP(z_align, pZ);
  return (float32_t*)pDst;
} /* fir_interpf() */
#elif HAVE_FPU
// for scalar FPU
float32_t * fir_interpf_4x(float32_t * restrict z, float32_t * restrict delay, float32_t * restrict p, const float32_t * restrict h, const float32_t * restrict x, int M, int N)
{
  int n, m;
  const xtfloat*  restrict pX = (const xtfloat*)x;
  const xtfloat*  restrict px = (const xtfloat*)x;
  const xtfloat* restrict pD = (const xtfloat*)p;
  const xtfloat*   restrict pH = (const xtfloat*)h;
  xtfloat*          pZ = (xtfloat*)z;
  NASSERT(x);
  NASSERT(z);
  NASSERT(N>0);
  NASSERT(M>0);
  NASSERT(M % 4 == 0);
  NASSERT(N % 8 == 0);
  WUR_AE_CBEGIN0((uintptr_t)(delay));
  WUR_AE_CEND0((uintptr_t)(delay + M));
  for (n = 0; n<N; n +=2)
  {
    xtfloat x0, x1;
    xtfloat H0, H1;
    xtfloat A0, A1, A2, A3;
    xtfloat s0, s1;
    pH = (const xtfloat*)h;
    {
      xtfloat temp;
      x0 = XT_LSI(pX, 0);
      x1 = XT_LSI(pX, 4);
      A0 = 
      A1 = 
      A2 = 
      A3 = XT_CONST_S(0);
      p = (float32_t*)pD;
      XT_LSXC(temp, pD, -4);
      __Pragma("loop_count min=1")
      for (m = 0; m<M; m ++)
      {
        H0 = pH[m + 0*M];
        H1 = pH[m + 1*M];
        XT_MADD_S(A0, H0, x0);
        XT_MADD_S(A1, H0, x1);
        XT_MADD_S(A2, H1, x0);
        XT_MADD_S(A3, H1, x1);
        x1 = x0;
        XT_LSXC(x0, pD, -4);
      }

      XT_LSXC(temp, pD, 4);
      XT_SSXP(A0, pZ, 4 );
      XT_SSXP(A2, pZ, 3*4);
      XT_SSXP(A1, pZ, 4);
      XT_SSXP(A3, pZ, -3*4);

      XT_LSIP(x0, pX, 4);
      XT_LSIP(x1, pX, 4);
      A0 =
      A1 =
      A2 =
      A3 = XT_CONST_S(0);
      pD = (const xtfloat*)p;
      XT_LSXC(temp, pD, -4);
      __Pragma("loop_count min=1")
      for (m = 0; m<M; m++)
      {
        H0 = pH[m + 2 * M];
        H1 = pH[m + 3 * M];
        XT_MADD_S(A0, H0, x0);
        XT_MADD_S(A1, H0, x1);
        XT_MADD_S(A2, H1, x0);
        XT_MADD_S(A3, H1, x1);
        x1 = x0;
        XT_LSXC(x0, pD, -4);
      }

      XT_LSXC(temp, pD, 4);
      XT_SSXP(A0, pZ, 4);
      XT_SSXP(A2, pZ, 3 * 4);
      XT_SSXP(A1, pZ, 4);
      XT_SSXP(A3, pZ, 4);
    }

    XT_LSIP(s0, castxcc(xtfloat, px), 4);
    XT_LSIP(s1, castxcc(xtfloat, px), 4);
    XT_SSXC(s0, castxcc(xtfloat, pD), 4);
    XT_SSXC(s1, castxcc(xtfloat, pD), 4);
  }
  return (float32_t*)pD;
} 
#endif
