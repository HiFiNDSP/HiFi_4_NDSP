/* ------------------------------------------------------------------------ */
/* Copyright (c) 2018 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
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
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */

/*
    NatureDSP Signal Processing Library. IIR part
    Biquad Real Block IIR, 32x16-bit, Direct Form I
    C code optimized for HiFi4
    IntegrIT, 2006-2014
*/

/* Portable data types. */
#include "NatureDSP_types.h"
/* Signal Processing Library API. */
#include "NatureDSP_Signal_iir.h"
/* Common utility and macros declarations. */
#include "common.h"
/* Filter instance structure. */
#include "bqriir32x16_df1_common.h"

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
void bqriir32x16_df1( bqriir32x16_df1_handle_t _bqriir,
                              void    *                s,
                              int32_t * restrict       r,
                        const int32_t *                x , int N)
{
#if (ALG32X16_DF1_ND==1)
  bqriir32x16_df1_nd(_bqriir, s, r, x, N);
#else
  bqriir32x16_df1_ptr_t bqriir = (bqriir32x16_df1_ptr_t)_bqriir;

  const ae_int16x4 *          coef;
        ae_int32x2 * restrict state;
  const ae_int32x2 * restrict px;
        ae_int32x2 * restrict pr;
  ae_valign aR;
  ae_valign aX;
 
  int M;
  int m, n;
  (void)s;
  NASSERT( bqriir && bqriir->magic == MAGIC && r && x );

  M = bqriir->M;
  NASSERT( N%2==0 );
  if (N<=0) return;
  coef  = (const ae_int16x4*)bqriir->coef;
  state = (      ae_int32x2*)bqriir->state;

  px = (const ae_int32x2*)x;
  pr = (      ae_int32x2*)r;

  //
  // Perform data block processing for each pair of successive sections. Use the
  // output array r[N] for temporal storage of inter-section signal.
  //

  for ( m=0; m<M/2; m++ )
  {
    ae_int16x4 cf0_gg0b0, cf0_b2b1a2a1, cf0_b1b0a2a1;
    ae_int16x4 cf1_gg0b0, cf1_b2b1a2a1, cf1_b1b0a2a1;

    // accXY: X - section id, Y - sample id
    ae_f64 acc00, acc01, acc10, acc11;

    // State variables, _ff - feedforward state, _fb - feedback state
    ae_f32x2 st0_ff, st0_fb, st1_ff, st1_fb;

    // Paired input samples for the first and second sections
    ae_int32x2 x01, y01;

    //
    // Load coefficients for the m-th pair of sections.
    //

    AE_L16X4_IP( cf0_gg0b0   , coef, +4*2 );
    AE_L16X4_IP( cf0_b2b1a2a1, coef, +4*2 );
    AE_L16X4_IP( cf0_b1b0a2a1, coef, +4*2 );
    AE_L16X4_IP( cf1_gg0b0   , coef, +4*2 );
    AE_L16X4_IP( cf1_b2b1a2a1, coef, +4*2 );
    AE_L16X4_IP( cf1_b1b0a2a1, coef, +4*2 );

    //
    // Load sections' state.
    //

    st0_ff = AE_L32X2_I( state, 0*2*4 );
    st0_fb = AE_L32X2_I( state, 1*2*4 );
    st1_ff = AE_L32X2_I( state, 2*2*4 );
    st1_fb = AE_L32X2_I( state, 3*2*4 );

    //
    // Pass N/2 sample pairs through 2 sections.
    //
    aX=AE_LA64_PP(px);
    aR=AE_ZALIGN64();
    __Pragma( "swp_schedule ii=12, unroll=2, sched[32]= 0 2 4 6 8 4 9 11 13 17 18 20 19 21 23 25 6 8 10 12 14 10 15 17 19 23 24 26 25 27 29 31" );
    for ( n=0; n<N/2; n++ )
    {
      AE_LA32X2_IP( x01, aX,px);

      // Q31 <- Q31*Q15 - 15 w/ rounding, saturation
      x01 = AE_MULFP32X16X2RAS_H( x01, cf0_gg0b0 );

      // Q46 <- Q31*Q14 + 1
      acc00 = AE_MULF32X16_H0( x01, cf0_gg0b0 );
      AE_MULAAAAFQ32X16( acc00, st0_ff, st0_fb, cf0_b2b1a2a1 );

      // Q31 <- Q46 + 1 - 16 w/ rounding, saturation
      AE_PKSR32( st0_fb, acc00, 1 );

      acc01 = AE_MULF32X16_L3( st0_ff, cf0_b2b1a2a1 );
      AE_MULAAAAFQ32X16( acc01, x01, st0_fb, cf0_b1b0a2a1 );

      st0_ff = x01;

      AE_PKSR32( st0_fb, acc01, 1 );

      y01 = AE_MULFP32X16X2RAS_H( st0_fb, cf1_gg0b0 );

      acc10 = AE_MULF32X16_H0( y01, cf1_gg0b0 );
      AE_MULAAAAFQ32X16( acc10, st1_ff, st1_fb, cf1_b2b1a2a1 );
      AE_PKSR32( st1_fb, acc10, 1 );

      acc11 = AE_MULF32X16_L3( st1_ff, cf1_b2b1a2a1 );
      AE_MULAAAAFQ32X16( acc11, y01, st1_fb, cf1_b1b0a2a1 );

      st1_ff = y01;

      AE_PKSR32( st1_fb, acc11, 1 );

      AE_SA32X2_IP( st1_fb, aR,pr);
    }
    AE_SA64POS_FP(aR,pr);
    //
    // Save sections' state.
    //

    AE_S32X2_IP( st0_ff, state, +2*4 );
    AE_S32X2_IP( st0_fb, state, +2*4 );
    AE_S32X2_IP( st1_ff, state, +2*4 );
    AE_S32X2_IP( st1_fb, state, +2*4 );

    // Second to last pair of sections are fed with output signal of the
    // previous pair.
    px = pr = (ae_int32x2*)r;
  }

  //
  // Process the last section if the number of biquads is odd.
  //

  if ( M & 1 )
  {
    ae_int16x4 cf_gg0b0, cf_b2b1a2a1, cf_b1b0a2a1;

    ae_f64 acc0, acc1;

    ae_f32x2 st_ff, st_fb;

    ae_int32x2 x01;

    //
    // Load coefficients for the last section.
    //

    cf_gg0b0    = AE_L16X4_I( coef, 0*4*2 );
    cf_b2b1a2a1 = AE_L16X4_I( coef, 1*4*2 );
    cf_b1b0a2a1 = AE_L16X4_I( coef, 2*4*2 );

    //
    // Load last section's state.
    //

    st_ff = AE_L32X2_I( state, 0*2*4 );
    st_fb = AE_L32X2_I( state, 1*2*4 );
  
    //
    // Pass N/2 sample pairs through the last section.
    //
    aX=AE_LA64_PP(px);
    aR=AE_ZALIGN64();

    __Pragma( "loop_count min=1" );
    for ( n=0; n<N/2; n++ )
    {
      AE_LA32X2_IP( x01, aX,px);

      // Q31 <- Q31*Q15 - 15 w/ rounding, saturation
      x01 = AE_MULFP32X16X2RAS_H( x01, cf_gg0b0 );

      // Q46 <- Q31*Q14 + 1
      acc0 = AE_MULF32X16_H0( x01, cf_gg0b0 );
      AE_MULAAAAFQ32X16( acc0, st_ff, st_fb, cf_b2b1a2a1 );

      // Q31 <- Q46 + 1 - 16 w/ rounding, saturation
      AE_PKSR32( st_fb, acc0, 1 );

      acc1 = AE_MULF32X16_L3( st_ff, cf_b2b1a2a1 );
      AE_MULAAAAFQ32X16( acc1, x01, st_fb, cf_b1b0a2a1 );

      st_ff = x01;

      AE_PKSR32( st_fb, acc1, 1 );

      AE_SA32X2_IP( st_fb, aR,pr);
    }
    AE_SA64POS_FP(aR,pr);

    //
    // Save section's state.
    //

    AE_S32X2_I( st_ff, state, 0*2*4 );
    AE_S32X2_I( st_fb, state, 1*2*4 );
  }

  //
  // Scale output data, if required.
  //

  if ( bqriir->gain != 0 )
  {
    WUR_AE_SAR( bqriir->gain );
    px = (const ae_int32x2*)r;
    pr = (      ae_int32x2*)r;
    if ((((uintptr_t)r)&(sizeof(ae_int32x2)-1))==0)
    {
        ae_int32x2 t0, t1;
        pr++;
        __Pragma("ymemory(pr)");
        for ( n=0; n<(N>>2); n++ )
        {
          AE_L32X2_IP( t0, px, sizeof(ae_int32x2) );
          AE_L32X2_IP( t1, px, sizeof(ae_int32x2) );

          t0 = AE_SLAS32S( t0 );
          t1 = AE_SLAS32S( t1 );

          AE_S32X2_I ( t0, pr, -(int)sizeof(ae_int32x2) );
          AE_S32X2_IP( t1, pr, 2*sizeof(ae_int32x2) );
        }

        if ( N & 2 )
        {
          t0 = AE_L32X2_I( px, 0 );
          t0 = AE_SLAS32S( t0 );
          AE_S32X2_I( t0, pr, -(int)sizeof(ae_int32x2) );
        }
    }
    else
    {
        ae_int32x2 t0;
        /* output pointer is not aligned by 8-byte boundary */
        AE_L32_IP( t0, castxcc(ae_int32,px), sizeof(ae_int32) );
        t0 = AE_SLAS32S( t0 );
        AE_S32_L_IP( t0, castxcc(ae_int32,pr), sizeof(ae_int32) );
        t0 = AE_L32_X( (ae_int32 *)px, (N-2)*sizeof(ae_int32) );
        t0 = AE_SLAS32S( t0 );
        AE_S32_L_X( t0, (ae_int32 *)pr, (N-2)*sizeof(ae_int32) );
        N -= 2;
        for ( n=0; n<(N>>1); n++ )
        {
          AE_L32X2_IP( t0, px, sizeof(ae_int32x2) );
          t0 = AE_SLAS32S( t0 );
          AE_S32X2_IP( t0, pr, sizeof(ae_int32x2) );
        }
    }
  }
  #endif
} // bqriir32x16_df1_process()
