/* ------------------------------------------------------------------------ */
/* Copyright (c) 2019 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
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
/*          Copyright (C) 2015-2019 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#include "NatureDSP_types.h"
#include "NatureDSP_Signal_img.h"
#include "common.h"
#include "img_common.h"

/*-------------------------------------------------------------------------
  RGB<->YUV conversion 
  Functions convert RGB planar images to YUV planar. 

  Y=c0*R+c1*G+c2*B
  U=c3*R+c4*G+c5*B + bias
  V=c6*R+c7*G+c8*B + bias
  where bias is 128 for 8-bit data and 16384 for 16-bit data

  Image formats:
  8-bit unsigned RGB/YUV
  16-bit signed RGB/YUV

  Input:
  inImgR,
  inImgG,
  inImgB  planes with R,G,B components
  c[13]   transform coefficients, Q29 format
  sz      image size
  Output:
  outImgY
  outImgU
  outImgV planes with Y,U,V components

  Restrictions:
  see general restrictions applied for all images for fast/generic 
  functions
-------------------------------------------------------------------------*/
void imgconvert_rgbyuv16( void * restrict outImgY, 
                          void * restrict outImgU, 
                          void * restrict outImgV, 
                          const void * restrict inImgR, 
                          const void * restrict inImgG, 
                          const void * restrict inImgB, 
                          const int32_t * restrict c,
                          const imgsize_t* sz)
{
    const ae_int16x4 * restrict pInR;
    const int16_t * restrict inR = (const int16_t *)inImgR;
    const ae_int16x4 * restrict pInG;
    const int16_t * restrict inG = (const int16_t *)inImgG;
    const ae_int16x4 * restrict pInB;
    const int16_t * restrict inB = (const int16_t *)inImgB;
    ae_int16x4 * restrict pOutY;
    int16_t * restrict outY = (int16_t *)outImgY;
    ae_int16x4 * restrict pOutU;
    int16_t * restrict outU = (int16_t *)outImgU;
    ae_int16x4 * restrict pOutV;
    int16_t * restrict outV = (int16_t *)outImgV;
    ae_int16x4 r, g, b, y, u, v;
    ae_int32x2 dr0, dr1, dg0, dg1, db0, db1, tmp;
    int i, j, w = (int)sz->width, h = (int)sz->height, stride = sz->stride;
    ae_int16x4 c0, c1, c2, c3, c4, c5, c6, c7, c8;
    ae_valign aly, alu, alv, alr, alg, alb;
    NASSERT(outImgY);
    NASSERT(outImgU);
    NASSERT(outImgV);
    NASSERT(inImgR);
    NASSERT(inImgG);
    NASSERT(inImgB);
    NASSERT_ALIGN(outImgY, 2);
    NASSERT_ALIGN(outImgU, 2);
    NASSERT_ALIGN(outImgV, 2);
    NASSERT_ALIGN(inImgR, 2);
    NASSERT_ALIGN(inImgG, 2);
    NASSERT_ALIGN(inImgB, 2);
    imgsize_validate(sz, 2, 0);
    tmp = AE_L32_X((const ae_int32 *)c, 0 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c0 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 1 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c1 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 2 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c2 = AE_ROUND16X4F32SASYM(tmp, tmp);
    /*Y*/
    aly = AE_ZALIGN64();
    for (i = 0; i < h; i++)
    {
        pInR = (const ae_int16x4*)(inR + i*stride);
        pInG = (const ae_int16x4*)(inG + i*stride);
        pInB = (const ae_int16x4*)(inB + i*stride);
        alr = AE_LA64_PP(pInR);
        alg = AE_LA64_PP(pInG);
        alb = AE_LA64_PP(pInB);
        pOutY = (ae_int16x4 *)(outY + i*stride);
        for (j = 0; j < (w >> 2); j++)
        {
            AE_LA16X4_IP(r, alr, pInR);
            AE_LA16X4_IP(g, alg, pInG);
            AE_LA16X4_IP(b, alb, pInB);

            AE_MOVT16X4(r, 0, AE_LT16(r, 0));
            AE_MOVT16X4(g, 0, AE_LT16(g, 0));
            AE_MOVT16X4(b, 0, AE_LT16(b, 0));

            AE_MUL16X4(dr0, dr1, r, c0);
            AE_MULA16X4(dr0, dr1, g, c1);
            AE_MULA16X4(dr0, dr1, b, c2);
            dr0 = AE_ADD32(dr0, dr0);
            dr1 = AE_ADD32(dr1, dr1);
            y = AE_ROUND16X4F32SASYM(dr0, dr1);
            AE_SA16X4_IP(y, aly, pOutY);
        }
        AE_SA64POS_FP(aly, pOutY);
    }
    /*U*/
    tmp = AE_L32_X((const ae_int32 *)c, 3 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c3 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 4 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c4 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 5 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c5 = AE_ROUND16X4F32SASYM(tmp, tmp);    
    alu = AE_ZALIGN64();
    for (i = 0; i < h; i++)
    {
        pInR = (const ae_int16x4*)(inR + i*stride);
        pInG = (const ae_int16x4*)(inG + i*stride);
        pInB = (const ae_int16x4*)(inB + i*stride);
        alr = AE_LA64_PP(pInR);
        alg = AE_LA64_PP(pInG);
        alb = AE_LA64_PP(pInB);
        pOutU = (ae_int16x4 *)(outU + i*stride);
        for (j = 0; j < (w >> 2); j++)
        {
            AE_LA16X4_IP(r, alr, pInR);
            AE_LA16X4_IP(g, alg, pInG);
            AE_LA16X4_IP(b, alb, pInB);
            AE_MOVT16X4(r, 0, AE_LT16(r, 0));
            AE_MOVT16X4(g, 0, AE_LT16(g, 0));
            AE_MOVT16X4(b, 0, AE_LT16(b, 0));

            AE_MUL16X4(dr0, dr1, r, c3);
            AE_MULA16X4(dr0, dr1, g, c4);
            AE_MULA16X4(dr0, dr1, b, c5);
            dr0 = AE_ADD32(dr0, dr0);
            dr1 = AE_ADD32(dr1, dr1);
            u = AE_ROUND16X4F32SASYM(dr0, dr1);

            u = AE_ADD16S(u, (ae_int16x4)16384);
            AE_MOVT16X4(u, 0, AE_LT16(u, 0));

            AE_SA16X4_IP(u, alu, pOutU);
        }
        AE_SA64POS_FP(alu, pOutU);
    }
    /*V*/
    alv = AE_ZALIGN64();
    tmp = AE_L32_X((const ae_int32 *)c, 6 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c6 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 7 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c7 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 8 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c8 = AE_ROUND16X4F32SASYM(tmp, tmp);
    for (i = 0; i < h; i++)
    {
        pInR = (const ae_int16x4*)(inR + i*stride);
        pInG = (const ae_int16x4*)(inG + i*stride);
        pInB = (const ae_int16x4*)(inB + i*stride);
        alr = AE_LA64_PP(pInR);
        alg = AE_LA64_PP(pInG);
        alb = AE_LA64_PP(pInB);
        pOutV = (ae_int16x4 *)(outV + i*stride);
        for (j = 0; j < (w >> 2); j++)
        {
            AE_LA16X4_IP(r, alr, pInR);
            AE_LA16X4_IP(g, alg, pInG);
            AE_LA16X4_IP(b, alb, pInB);
            AE_MOVT16X4(r, 0, AE_LT16(r, 0));
            AE_MOVT16X4(g, 0, AE_LT16(g, 0));
            AE_MOVT16X4(b, 0, AE_LT16(b, 0));

            AE_MUL16X4(dr0, dr1, r, c6);
            AE_MULA16X4(dr0, dr1, g, c7);
            AE_MULA16X4(dr0, dr1, b, c8);;
            dr0 = AE_ADD32(dr0, dr0);
            dr1 = AE_ADD32(dr1, dr1);
            v = AE_ROUND16X4F32SASYM(dr0, dr1);

            v = AE_ADD16S(v, (ae_int16x4)16384);
            AE_MOVT16X4(v, 0, AE_LT16(v, 0));

            AE_SA16X4_IP(v, alv, pOutV);
        }
        AE_SA64POS_FP(alv, pOutV);
    }
    tmp = AE_L32_X((const ae_int32 *)c, 0 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c0 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 1 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c1 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 2 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c2 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 3 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c3 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 4 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c4 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 5 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c5 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 6 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c6 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 7 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c7 = AE_ROUND16X4F32SASYM(tmp, tmp);
    tmp = AE_L32_X((const ae_int32 *)c, 8 * sizeof(int32_t));
    tmp = AE_SLLI32(tmp, 2);
    c8 = AE_ROUND16X4F32SASYM(tmp, tmp);
    for (i = 0; i < h; i++)
    {
        for (j = w&(~3); j<w; j++)
        {
            ae_int16x4 y, u, v;
            r = AE_MOVDA16(inR[i*stride + j]);
            g = AE_MOVDA16(inG[i*stride + j]);
            b = AE_MOVDA16(inB[i*stride + j]);
            AE_MOVT16X4(r, 0, AE_LT16(r, 0));
            AE_MOVT16X4(g, 0, AE_LT16(g, 0));
            AE_MOVT16X4(b, 0, AE_LT16(b, 0));

            AE_MUL16X4(dr0, dr1, r, c0);
            AE_MUL16X4(dg0, dg1, g, c1);
            AE_MUL16X4(db0, db1, b, c2);
            dr0 = AE_ADD32(dr0, dg0);
            db0 = AE_ADD32(db0, 1 << 14);
            dr0 = AE_ADD32(dr0, db0);
            dr1 = AE_ADD32(dr1, dg1);
            db1 = AE_ADD32(db1, 1 << 14);
            dr1 = AE_ADD32(dr1, db1);
            y = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            AE_MUL16X4(dr0, dr1, r, c3);
            AE_MUL16X4(dg0, dg1, g, c4);
            AE_MUL16X4(db0, db1, b, c5);
            dr0 = AE_ADD32(dr0, dg0);
            db0 = AE_ADD32(db0, 1 << 14);
            dr0 = AE_ADD32(dr0, db0);
            dr1 = AE_ADD32(dr1, dg1);
            db1 = AE_ADD32(db1, 1 << 14);
            dr1 = AE_ADD32(dr1, db1);
            u = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            AE_MUL16X4(dr0, dr1, r, c6);
            AE_MUL16X4(dg0, dg1, g, c7);
            AE_MUL16X4(db0, db1, b, c8);
            dr0 = AE_ADD32(dr0, dg0);
            db0 = AE_ADD32(db0, 1 << 14);
            dr0 = AE_ADD32(dr0, db0);
            dr1 = AE_ADD32(dr1, dg1);
            db1 = AE_ADD32(db1, 1 << 14);
            dr1 = AE_ADD32(dr1, db1);
            v = AE_SAT16X4(AE_SRAI32(dr0, 15), AE_SRAI32(dr1, 15));

            u = AE_ADD16S(u, (ae_int16x4)16384);
            v = AE_ADD16S(v, (ae_int16x4)16384);
            AE_MOVT16X4(v, 0, AE_LT16(v, 0));
            AE_MOVT16X4(u, 0, AE_LT16(u, 0));
            outY[i*stride + j] = AE_MOVAD16_0(y);
            outU[i*stride + j] = AE_MOVAD16_0(u);
            outV[i*stride + j] = AE_MOVAD16_0(v);
        }
    }
}
