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
* Test module for testing cycle performance (Mixed Radix FFT)
*/

/* Cross-platform data type definitions. */
#include "types.h"
/* Test environment configuration. */
#include "config.h"
/* Filters and transformations API. */
#include LIBRARY_HEADER(fft)
/* Measurement utilities */
#include "mips.h"
 
#define PROFILE_INVERTED_FFT_cfft32x32(cond,verb,N,i)   PROFILE_INVERTED_FFT_SC( cond,verb,fft_cplx32x32,N,i,( mips.out0.i32, mips.inp1.i32, cnfft32_##N, i),fout,prf_ptscycle3,N );
#define PROFILE_INVERTED_FFT_cifft32x32(cond,verb,N,i)   PROFILE_INVERTED_FFT_SC( cond,verb,ifft_cplx32x32,N,i,( mips.out0.i32, mips.inp1.i32, cinfft32_##N, i),fout,prf_ptscycle3,N );
#define PROFILE_INVERTED_FFT_cfft32x16(cond,verb,N,i)   PROFILE_INVERTED_FFT_SC( cond,verb,fft_cplx32x16,N,i,( mips.out0.i32, mips.inp1.i32, cnfft32x16_##N, i),fout,prf_ptscycle3,N );
#define PROFILE_INVERTED_FFT_cifft32x16(cond,verb,N,i)   PROFILE_INVERTED_FFT_SC( cond,verb,ifft_cplx32x16,N,i,( mips.out0.i32, mips.inp1.i32, cinfft32x16_##N, i),fout,prf_ptscycle3,N );
#define PROFILE_INVERTED_FFT_cfft16x16(cond,verb,N,i)   PROFILE_INVERTED_FFT_SC( cond,verb,fft_cplx16x16,N,i,( mips.out0.i16, mips.inp1.i16, cnfft16_##N, i),fout,prf_ptscycle3,N );
#define PROFILE_INVERTED_FFT_cifft16x16(cond,verb,N,i)   PROFILE_INVERTED_FFT_SC( cond,verb,ifft_cplx16x16,N,i,( mips.out0.i16, mips.inp1.i16, cinfft16_##N, i),fout,prf_ptscycle3,N );

void mips_cnfft(int phaseNum, int isFull, int isVerbose, FILE* fout)
{
    if ( phaseNum == 0 || phaseNum == 1 )
    {
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 12, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 12, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 24, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 24, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 36, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 36, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 48, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 48, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 60, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 60, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 72, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 72, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 80, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 80, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 96, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 96, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 100, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 100, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 108, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 108, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 120, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 120, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 144, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 144, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 160, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 160, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 180, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 180, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 192, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 192, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 200, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 200, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 216, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 216, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 240, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 240, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 288, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 288, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 300, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 300, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 320, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 320, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 324, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 324, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 360, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 360, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 384, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 384, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 400, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 400, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 432, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 432, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 480, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 480, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 540, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 540, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 576, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 576, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 600, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 600, 2);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 768, 3);
        PROFILE_INVERTED_FFT_cfft32x32(isFull, isVerbose, 768, 2);
        PROFILE_INVERTED_FFT_cfft32x32(1     , isVerbose, 960, 3);
        PROFILE_INVERTED_FFT_cfft32x32(1     , isVerbose, 960, 2);

        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 12, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 12, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 24, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 24, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 36, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 36, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 48, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 48, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 60, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 60, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 72, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 72, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 80, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 80, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 96, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 96, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 100, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 100, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 108, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 108, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 120, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 120, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 144, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 144, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 160, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 160, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 180, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 180, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 192, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 192, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 200, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 200, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 216, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 216, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 240, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 240, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 288, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 288, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 300, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 300, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 320, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 320, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 324, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 324, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 360, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 360, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 384, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 384, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 400, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 400, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 432, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 432, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 480, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 480, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 540, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 540, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 576, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 576, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 600, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 600, 2);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 768, 3);
        PROFILE_INVERTED_FFT_cifft32x32(isFull, isVerbose, 768, 2);
        PROFILE_INVERTED_FFT_cifft32x32(1     , isVerbose, 960, 3);
        PROFILE_INVERTED_FFT_cifft32x32(1     , isVerbose, 960, 2);

        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 160, 3);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 160, 2);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 192, 3);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 192, 2);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 240, 3);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 240, 2);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 320, 3);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 320, 2);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 384, 3);
        PROFILE_INVERTED_FFT_cfft32x16(isFull, isVerbose, 384, 2);
        PROFILE_INVERTED_FFT_cfft32x16(1     , isVerbose, 480, 3);
        PROFILE_INVERTED_FFT_cfft32x16(1     , isVerbose, 480, 2);

        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 160, 3);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 160, 2);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 192, 3);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 192, 2);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 240, 3);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 240, 2);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 320, 3);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 320, 2);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 384, 3);
        PROFILE_INVERTED_FFT_cifft32x16(isFull, isVerbose, 384, 2);
        PROFILE_INVERTED_FFT_cifft32x16(1     , isVerbose, 480, 3);
        PROFILE_INVERTED_FFT_cifft32x16(1     , isVerbose, 480, 2);

        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 160, 3);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 160, 2);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 192, 3);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 192, 2);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 240, 3);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 240, 2);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 320, 3);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 320, 2);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 384, 3);
        PROFILE_INVERTED_FFT_cfft16x16(isFull, isVerbose, 384, 2);
        PROFILE_INVERTED_FFT_cfft16x16(1     , isVerbose, 480, 3);
        PROFILE_INVERTED_FFT_cfft16x16(1     , isVerbose, 480, 2);

        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 160, 3);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 160, 2);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 192, 3);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 192, 2);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 240, 3);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 240, 2);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 320, 3);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 320, 2);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 384, 3);
        PROFILE_INVERTED_FFT_cifft16x16(isFull, isVerbose, 384, 2);
        PROFILE_INVERTED_FFT_cifft16x16(1     , isVerbose, 480, 3);
        PROFILE_INVERTED_FFT_cifft16x16(1     , isVerbose, 480, 2);

    }

} /* mips_cnfft() */
