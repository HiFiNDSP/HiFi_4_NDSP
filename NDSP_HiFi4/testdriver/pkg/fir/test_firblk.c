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
 * Test procedures for FIR
 */

/* Cross-platform data type definitions. */
#include "types.h"
/* Test environment configuration. */
#include "config.h"
/* DSP Library API */
#include LIBRARY_HEADER(fir)
/* Test data vectors tools and SEQ-file reader. */
#include "vectools.h"
#include "testeng_fir.h"
#include "testeng_fir_old.h"

#include <string.h>
#include <stdlib.h>
#if 0   // for HiFi3/3z
static tFirOldDescr api_bkfir24x24    ={{(tFirOldFxnAlloc*)bkfir24x24_alloc,    (tFirOldFxnInit*)bkfir24x24_init,    (tFirOldFxnProcess*)bkfir24x24_process    ,(tFirOldextIrFxnAllocExtIr*)bkfir24x24_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)bkfir24x24_copyExtIr }};
static tFirOldDescr api_bkfira24x24   ={{(tFirOldFxnAlloc*)bkfira24x24_alloc,   (tFirOldFxnInit*)bkfira24x24_init,   (tFirOldFxnProcess*)bkfira24x24_process   }};
static tFirOldDescr api_cxfir24x24    ={{(tFirOldFxnAlloc*)cxfir24x24_alloc,    (tFirOldFxnInit*)cxfir24x24_init,    (tFirOldFxnProcess*)cxfir24x24_process    ,(tFirOldextIrFxnAllocExtIr*)cxfir24x24_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)cxfir24x24_copyExtIr }};
static const tTestEngDesc descr_cxfir24x24      = { FMT_CPLX | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfira24x24     = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDGENERIC                    , NULL,TE_DIM_NUM_1, TE_ALIGN_NO , te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
#endif
static const tTestEngDesc descr_bkfir24x24      = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };

/* phase 1*/
static tFirOldDescr api_bkfir24x24p   ={{(tFirOldFxnAlloc*)bkfir24x24p_alloc,   (tFirOldFxnInit*)bkfir24x24p_init,   (tFirOldFxnProcess*)bkfir24x24p_process   ,(tFirOldextIrFxnAllocExtIr*)bkfir24x24p_allocExtIr,(tFirOldextIrFxnCopyExtIr*)bkfir24x24p_copyExtIr}};
static tFirOldDescr api_bkfir16x16    ={{(tFirOldFxnAlloc*)bkfir16x16_alloc,    (tFirOldFxnInit*)bkfir16x16_init,    (tFirOldFxnProcess*)bkfir16x16_process    ,(tFirOldextIrFxnAllocExtIr*)bkfir16x16_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)bkfir16x16_copyExtIr }};
static tFirOldDescr api_bkfir32x16    ={{(tFirOldFxnAlloc*)bkfir32x16_alloc,    (tFirOldFxnInit*)bkfir32x16_init,    (tFirOldFxnProcess*)bkfir32x16_process    ,(tFirOldextIrFxnAllocExtIr*)bkfir32x16_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)bkfir32x16_copyExtIr }};
static tFirOldDescr api_bkfir32x32    ={{(tFirOldFxnAlloc*)bkfir32x32_alloc,    (tFirOldFxnInit*)bkfir32x32_init,    (tFirOldFxnProcess*)bkfir32x32_process    ,(tFirOldextIrFxnAllocExtIr*)bkfir32x32_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)bkfir32x32_copyExtIr }};
static tFirOldDescr api_bkfir32x32ep  ={{(tFirOldFxnAlloc*)bkfir32x32ep_alloc,  (tFirOldFxnInit*)bkfir32x32ep_init,  (tFirOldFxnProcess*)bkfir32x32ep_process  ,(tFirOldextIrFxnAllocExtIr*)bkfir32x32_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)bkfir32x32_copyExtIr }};
static tFirOldDescr api_cxfir16x16    ={{(tFirOldFxnAlloc*)cxfir16x16_alloc,    (tFirOldFxnInit*)cxfir16x16_init,    (tFirOldFxnProcess*)cxfir16x16_process    ,(tFirOldextIrFxnAllocExtIr*)cxfir16x16_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)cxfir16x16_copyExtIr }};
static tFirOldDescr api_cxfir32x16    ={{(tFirOldFxnAlloc*)cxfir32x16_alloc,    (tFirOldFxnInit*)cxfir32x16_init,    (tFirOldFxnProcess*)cxfir32x16_process    ,(tFirOldextIrFxnAllocExtIr*)cxfir32x16_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)cxfir32x16_copyExtIr }};
static tFirOldDescr api_cxfir32x32    ={{(tFirOldFxnAlloc*)cxfir32x32_alloc,    (tFirOldFxnInit*)cxfir32x32_init,    (tFirOldFxnProcess*)cxfir32x32_process    ,(tFirOldextIrFxnAllocExtIr*)cxfir32x32_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)cxfir32x32_copyExtIr }};
static tFirOldDescr api_cxfir32x32ep  ={{(tFirOldFxnAlloc*)cxfir32x32ep_alloc,  (tFirOldFxnInit*)cxfir32x32ep_init,  (tFirOldFxnProcess*)cxfir32x32ep_process  ,(tFirOldextIrFxnAllocExtIr*)cxfir32x32_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)cxfir32x32ep_copyExtIr }};
static tFirOldDescr api_bkfira16x16   ={{(tFirOldFxnAlloc*)bkfira16x16_alloc,   (tFirOldFxnInit*)bkfira16x16_init,   (tFirOldFxnProcess*)bkfira16x16_process   }};
static tFirOldDescr api_bkfira32x16   ={{(tFirOldFxnAlloc*)bkfira32x16_alloc,   (tFirOldFxnInit*)bkfira32x16_init,   (tFirOldFxnProcess*)bkfira32x16_process   }};
static tFirOldDescr api_bkfira32x32   ={{(tFirOldFxnAlloc*)bkfira32x32_alloc,   (tFirOldFxnInit*)bkfira32x32_init,   (tFirOldFxnProcess*)bkfira32x32_process   }};
static tFirOldDescr api_bkfira32x32ep ={{(tFirOldFxnAlloc*)bkfira32x32ep_alloc, (tFirOldFxnInit*)bkfira32x32ep_init, (tFirOldFxnProcess*)bkfira32x32ep_process }};
static tFirStereoDescr api_stereo_bkfir16x16    ={{(tFirStereoFxnAlloc*)stereo_bkfir16x16_alloc,    (tFirStereoFxnInit*)stereo_bkfir16x16_init,    (tFirStereoFxnProcess*)stereo_bkfir16x16_process    ,(tFirStereoextIrFxnAllocExtIr*)stereo_bkfir16x16_allocExtIr ,(tFirStereoextIrFxnCopyExtIr*)stereo_bkfir16x16_copyExtIr }};
static tFirStereoDescr api_stereo_bkfir32x32    ={{(tFirStereoFxnAlloc*)stereo_bkfir32x32_alloc,    (tFirStereoFxnInit*)stereo_bkfir32x32_init,    (tFirStereoFxnProcess*)stereo_bkfir32x32_process    ,(tFirStereoextIrFxnAllocExtIr*)stereo_bkfir32x32_allocExtIr ,(tFirStereoextIrFxnCopyExtIr*)stereo_bkfir32x32_copyExtIr }};
static const tTestEngDesc descr_bkfir16x16      = { FMT_REAL | FMT_FRACT16, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfir32x16      = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR|TE_FIR_FILTER_32X16  , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfir32x32      = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfir32x32ep    = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_cxfir16x16      = { FMT_CPLX | FMT_FRACT16, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_cxfir32x16      = { FMT_CPLX | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR|TE_FIR_FILTER_32X16  , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_cxfir32x32      = { FMT_CPLX | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_cxfir32x32ep    = { FMT_CPLX | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfira16x16     = { FMT_REAL | FMT_FRACT16, TE_FIR_FIR|TE_FIR_OLDGENERIC                    , NULL,TE_DIM_NUM_1, TE_ALIGN_NO , te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfira32x16     = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDGENERIC|TE_FIR_FILTER_32X16, NULL,TE_DIM_NUM_1, TE_ALIGN_NO , te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfira32x32     = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDGENERIC                    , NULL,TE_DIM_NUM_1, TE_ALIGN_NO , te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfira32x32ep   = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDGENERIC                    , NULL,TE_DIM_NUM_1, TE_ALIGN_NO , te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };

static const tTestEngDesc descr_stereo_bkfir16x16      = { FMT_REAL | FMT_FRACT16, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_stereo, te_destroy_fir_stereo, &te_loadFxn_fir_stereo, &te_processFxn_fir_stereo };
static const tTestEngDesc descr_stereo_bkfir32x32      = { FMT_REAL | FMT_FRACT32, TE_FIR_FIR|TE_FIR_OLDEXTIR                      , NULL,TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_stereo, te_destroy_fir_stereo, &te_loadFxn_fir_stereo, &te_processFxn_fir_stereo };
/* phase 2*/
static tFirOldDescr api_bkfirf        ={{(tFirOldFxnAlloc*)bkfirf_alloc,        (tFirOldFxnInit*)bkfirf_init,        (tFirOldFxnProcess*)bkfirf_process        ,(tFirOldextIrFxnAllocExtIr*)bkfirf_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)bkfirf_copyExtIr }};
static tFirOldDescr api_bkfiraf       ={{(tFirOldFxnAlloc*)bkfiraf_alloc,       (tFirOldFxnInit*)bkfiraf_init,       (tFirOldFxnProcess*)bkfiraf_process       }};
static tFirOldDescr api_cxfirf        ={{(tFirOldFxnAlloc*)cxfirf_alloc,        (tFirOldFxnInit*)cxfirf_init,        (tFirOldFxnProcess*)cxfirf_process        ,(tFirOldextIrFxnAllocExtIr*)cxfirf_allocExtIr ,(tFirOldextIrFxnCopyExtIr*)cxfirf_copyExtIr }};
static tFirStereoDescr api_stereo_bkfirf        ={{(tFirStereoFxnAlloc*)stereo_bkfirf_alloc,        (tFirStereoFxnInit*)stereo_bkfirf_init,        (tFirStereoFxnProcess*)stereo_bkfirf_process        ,(tFirStereoextIrFxnAllocExtIr*)stereo_bkfirf_allocExtIr ,(tFirStereoextIrFxnCopyExtIr*)stereo_bkfirf_copyExtIr }};
static const tTestEngDesc descr_bkfirf          = { FMT_REAL | FMT_FLOAT32, TE_FIR_FIR|TE_FIR_OLDEXTIR  , NULL, TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_bkfiraf         = { FMT_REAL | FMT_FLOAT32, TE_FIR_FIR|TE_FIR_OLDGENERIC, NULL, TE_DIM_NUM_1, TE_ALIGN_NO , te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_cxfirf          = { FMT_CPLX | FMT_FLOAT32, TE_FIR_FIR|TE_FIR_OLDEXTIR  , NULL, TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_old, te_destroy_fir_old, &te_loadFxn_fir_old, &te_processFxn_fir_old };
static const tTestEngDesc descr_stereo_bkfirf          = { FMT_REAL | FMT_FLOAT32, TE_FIR_FIR|TE_FIR_OLDEXTIR             , NULL, TE_DIM_NUM_1, TE_ALIGN_YES, te_create_fir_stereo, te_destroy_fir_stereo, &te_loadFxn_fir_stereo, &te_processFxn_fir_stereo };

typedef struct
{
  int                 phaseNum;
  const tTestEngDesc *pFirDescr;
  tTestEngTarget      fxns;
  int                 runAlways;   /* 1 - brief & full, 0 - full only */
  const char*         seqFile;
}
tTbl;

static const tTbl tests[] =
{
  /* Block Real FIR Filter */
  { 1, &descr_bkfir16x16, (tTestEngTarget)&api_bkfir16x16, 1,"bkfir16x16_lpf1.seq" },
  { 1, &descr_bkfir16x16, (tTestEngTarget)&api_bkfir16x16, 1,"bkfir16x16_bpf1.seq" },
  { 1, &descr_bkfir16x16, (tTestEngTarget)&api_bkfir16x16, 1,"bkfir16x16_4taps.seq"},
  { 1, &descr_bkfir16x16, (tTestEngTarget)&api_bkfir16x16, 0,"bkfir16x16_lpf2.seq" },
  { 1, &descr_bkfir16x16, (tTestEngTarget)&api_bkfir16x16, 0,"bkfir16x16_hpf1.seq" },
  { 1, &descr_bkfir16x16, (tTestEngTarget)&api_bkfir16x16, 0,"bkfir16x16_bpf2.seq" },

  { 1, &descr_bkfir32x16, (tTestEngTarget)&api_bkfir32x16, 1,"bkfir32x16_lpf1.seq" },
  { 1, &descr_bkfir32x16, (tTestEngTarget)&api_bkfir32x16, 1,"bkfir32x16_bpf1.seq" },
  { 1, &descr_bkfir32x16, (tTestEngTarget)&api_bkfir32x16, 1,"bkfir32x16_4taps.seq"},
  { 1, &descr_bkfir32x16, (tTestEngTarget)&api_bkfir32x16, 0,"bkfir32x16_lpf2.seq" },
  { 1, &descr_bkfir32x16, (tTestEngTarget)&api_bkfir32x16, 0,"bkfir32x16_hpf1.seq" },
  { 1, &descr_bkfir32x16, (tTestEngTarget)&api_bkfir32x16, 0,"bkfir32x16_bpf2.seq" },

  { 1, &descr_bkfir32x32, (tTestEngTarget)&api_bkfir32x32, 1,"bkfir32x32_lpf1.seq" },
  { 1, &descr_bkfir32x32, (tTestEngTarget)&api_bkfir32x32, 1,"bkfir32x32_bpf1.seq" },
  { 1, &descr_bkfir32x32, (tTestEngTarget)&api_bkfir32x32, 1,"bkfir32x32_4taps.seq"},
  { 1, &descr_bkfir32x32, (tTestEngTarget)&api_bkfir32x32, 0,"bkfir32x32_lpf2.seq" },
  { 1, &descr_bkfir32x32, (tTestEngTarget)&api_bkfir32x32, 0,"bkfir32x32_hpf1.seq" },
  { 1, &descr_bkfir32x32, (tTestEngTarget)&api_bkfir32x32, 0,"bkfir32x32_bpf2.seq" },

  { 1, &descr_bkfir32x32ep, (tTestEngTarget)&api_bkfir32x32ep, 1,"bkfir32x32ep_lpf1.seq" },
  { 1, &descr_bkfir32x32ep, (tTestEngTarget)&api_bkfir32x32ep, 1,"bkfir32x32ep_bpf1.seq" },
  { 1, &descr_bkfir32x32ep, (tTestEngTarget)&api_bkfir32x32ep, 1,"bkfir32x32ep_4taps.seq"},
  { 1, &descr_bkfir32x32ep, (tTestEngTarget)&api_bkfir32x32ep, 0,"bkfir32x32ep_lpf2.seq" },
  { 1, &descr_bkfir32x32ep, (tTestEngTarget)&api_bkfir32x32ep, 0,"bkfir32x32ep_hpf1.seq" },
  { 1, &descr_bkfir32x32ep, (tTestEngTarget)&api_bkfir32x32ep, 0,"bkfir32x32ep_bpf2.seq" },
#if 0 //for HiFi3/3z
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24, 1, "bkfir24x24_lpf1.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24, 1, "bkfir24x24_bpf1.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24, 1, "bkfir24x24_4taps.seq"},
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24, 0, "bkfir24x24_lpf2.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24, 0, "bkfir24x24_hpf1.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24, 0, "bkfir24x24_bpf2.seq" },

#endif
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24p, 1, "bkfir24x24p_lpf1.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24p, 1, "bkfir24x24p_bpf1.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24p, 1, "bkfir24x24p_4taps.seq"},
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24p, 0, "bkfir24x24p_lpf2.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24p, 0, "bkfir24x24p_hpf1.seq" },
  { 1, &descr_bkfir24x24, (tTestEngTarget)&api_bkfir24x24p, 0, "bkfir24x24p_bpf2.seq" },

  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 1,"bkfira16x16_lpf1.seq"  },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 1,"bkfira16x16_bpf1.seq"  },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 1,"bkfira16x16_3taps.seq" },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 0,"bkfira16x16_lpf2.seq"  },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 0,"bkfira16x16_hpf1.seq"  },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 0,"bkfira16x16_bpf2.seq"  },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 0,"bkfira16x16_4taps.seq" },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 0,"bkfira16x16_2taps.seq" },
  { 1, &descr_bkfira16x16, (tTestEngTarget)&api_bkfira16x16, 0,"bkfira16x16_1tap.seq"  },
#if 0// for HiFi3/3z
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 1,"bkfira24x24_lpf1.seq"  },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 1,"bkfira24x24_bpf1.seq"  },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 1,"bkfira24x24_3taps.seq" },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 0,"bkfira24x24_lpf2.seq"  },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 0,"bkfira24x24_hpf1.seq"  },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 0,"bkfira24x24_bpf2.seq"  },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 0,"bkfira24x24_4taps.seq" },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 0,"bkfira24x24_2taps.seq" },
  { 1, &descr_bkfira24x24, (tTestEngTarget)&api_bkfira24x24, 0,"bkfira24x24_1tap.seq"  },
#endif
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 1,"bkfira32x16_lpf1.seq"  },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 1,"bkfira32x16_bpf1.seq"  },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 1,"bkfira32x16_3taps.seq" },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 0,"bkfira32x16_lpf2.seq"  },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 0,"bkfira32x16_hpf1.seq"  },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 0,"bkfira32x16_bpf2.seq"  },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 0,"bkfira32x16_4taps.seq" },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 0,"bkfira32x16_2taps.seq" },
  { 1, &descr_bkfira32x16, (tTestEngTarget)&api_bkfira32x16, 0,"bkfira32x16_1tap.seq"  },

  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 1,"bkfira32x32_lpf1.seq"  },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 1,"bkfira32x32_bpf1.seq"  },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 1,"bkfira32x32_3taps.seq" },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 0,"bkfira32x32_lpf2.seq"  },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 0,"bkfira32x32_hpf1.seq"  },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 0,"bkfira32x32_bpf2.seq"  },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 0,"bkfira32x32_4taps.seq" },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 0,"bkfira32x32_2taps.seq" },
  { 1, &descr_bkfira32x32, (tTestEngTarget)&api_bkfira32x32, 0,"bkfira32x32_1tap.seq"  },

  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 1,"bkfira32x32ep_lpf1.seq"  },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 1,"bkfira32x32ep_bpf1.seq"  },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 1,"bkfira32x32ep_3taps.seq" },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 0,"bkfira32x32ep_lpf2.seq"  },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 0,"bkfira32x32ep_hpf1.seq"  },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 0,"bkfira32x32ep_bpf2.seq"  },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 0,"bkfira32x32ep_4taps.seq" },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 0,"bkfira32x32ep_2taps.seq" },
  { 1, &descr_bkfira32x32ep, (tTestEngTarget)&api_bkfira32x32ep, 0,"bkfira32x32ep_1tap.seq"  },

  { 1, &descr_stereo_bkfir16x16, (tTestEngTarget)&api_stereo_bkfir16x16, 1,"stereo_bkfir16x16_lpf1.seq" },
  { 1, &descr_stereo_bkfir16x16, (tTestEngTarget)&api_stereo_bkfir16x16, 1,"stereo_bkfir16x16_bpf1.seq" },
  { 1, &descr_stereo_bkfir16x16, (tTestEngTarget)&api_stereo_bkfir16x16, 1,"stereo_bkfir16x16_tap4.seq" },
  { 1, &descr_stereo_bkfir16x16, (tTestEngTarget)&api_stereo_bkfir16x16, 0,"stereo_bkfir16x16_lpf2.seq" },
  { 1, &descr_stereo_bkfir16x16, (tTestEngTarget)&api_stereo_bkfir16x16, 0,"stereo_bkfir16x16_hpf1.seq" },
  { 1, &descr_stereo_bkfir16x16, (tTestEngTarget)&api_stereo_bkfir16x16, 0,"stereo_bkfir16x16_bpf2.seq" },

  { 1, &descr_stereo_bkfir32x32, (tTestEngTarget)&api_stereo_bkfir32x32, 1,"stereo_bkfir32x32_lpf1.seq" },
  { 1, &descr_stereo_bkfir32x32, (tTestEngTarget)&api_stereo_bkfir32x32, 1,"stereo_bkfir32x32_bpf1.seq" },
  { 1, &descr_stereo_bkfir32x32, (tTestEngTarget)&api_stereo_bkfir32x32, 1,"stereo_bkfir32x32_tap4.seq" },
  { 1, &descr_stereo_bkfir32x32, (tTestEngTarget)&api_stereo_bkfir32x32, 0,"stereo_bkfir32x32_lpf2.seq" },
  { 1, &descr_stereo_bkfir32x32, (tTestEngTarget)&api_stereo_bkfir32x32, 0,"stereo_bkfir32x32_hpf1.seq" },
  { 1, &descr_stereo_bkfir32x32, (tTestEngTarget)&api_stereo_bkfir32x32, 0,"stereo_bkfir32x32_bpf2.seq" },

  /* Block Complex FIR Filter */
  { 1, &descr_cxfir16x16, (tTestEngTarget)&api_cxfir16x16, 1,"cxfir16x16_lpf1.seq" },
  { 1, &descr_cxfir16x16, (tTestEngTarget)&api_cxfir16x16, 1,"cxfir16x16_bpf1.seq" },
  { 1, &descr_cxfir16x16, (tTestEngTarget)&api_cxfir16x16, 1,"cxfir16x16_4taps.seq"},
  { 1, &descr_cxfir16x16, (tTestEngTarget)&api_cxfir16x16, 0,"cxfir16x16_lpf2.seq" },
  { 1, &descr_cxfir16x16, (tTestEngTarget)&api_cxfir16x16, 0,"cxfir16x16_hpf1.seq" },
  { 1, &descr_cxfir16x16, (tTestEngTarget)&api_cxfir16x16, 0,"cxfir16x16_bpf2.seq" },

  { 1, &descr_cxfir32x16, (tTestEngTarget)&api_cxfir32x16, 1,"cxfir32x16_lpf1.seq" },
  { 1, &descr_cxfir32x16, (tTestEngTarget)&api_cxfir32x16, 1,"cxfir32x16_bpf1.seq" },
  { 1, &descr_cxfir32x16, (tTestEngTarget)&api_cxfir32x16, 1,"cxfir32x16_4taps.seq"},
  { 1, &descr_cxfir32x16, (tTestEngTarget)&api_cxfir32x16, 0,"cxfir32x16_lpf2.seq" },
  { 1, &descr_cxfir32x16, (tTestEngTarget)&api_cxfir32x16, 0,"cxfir32x16_hpf1.seq" },
  { 1, &descr_cxfir32x16, (tTestEngTarget)&api_cxfir32x16, 0,"cxfir32x16_bpf2.seq" },
#if 0// for HiFi3/3z
  { 1, &descr_cxfir24x24, (tTestEngTarget)&api_cxfir24x24, 1,"cxfir24x24_lpf1.seq" },
  { 1, &descr_cxfir24x24, (tTestEngTarget)&api_cxfir24x24, 1,"cxfir24x24_bpf1.seq" },
  { 1, &descr_cxfir24x24, (tTestEngTarget)&api_cxfir24x24, 1,"cxfir24x24_4taps.seq"},
  { 1, &descr_cxfir24x24, (tTestEngTarget)&api_cxfir24x24, 0,"cxfir24x24_lpf2.seq" },
  { 1, &descr_cxfir24x24, (tTestEngTarget)&api_cxfir24x24, 0,"cxfir24x24_hpf1.seq" },
  { 1, &descr_cxfir24x24, (tTestEngTarget)&api_cxfir24x24, 0,"cxfir24x24_bpf2.seq" },
#endif
  { 1, &descr_cxfir32x32, (tTestEngTarget)&api_cxfir32x32, 1,"cxfir32x32_lpf1.seq" },
  { 1, &descr_cxfir32x32, (tTestEngTarget)&api_cxfir32x32, 1,"cxfir32x32_bpf1.seq" },
  { 1, &descr_cxfir32x32, (tTestEngTarget)&api_cxfir32x32, 1,"cxfir32x32_4taps.seq"},
  { 1, &descr_cxfir32x32, (tTestEngTarget)&api_cxfir32x32, 0,"cxfir32x32_lpf2.seq" },
  { 1, &descr_cxfir32x32, (tTestEngTarget)&api_cxfir32x32, 0,"cxfir32x32_hpf1.seq" },
  { 1, &descr_cxfir32x32, (tTestEngTarget)&api_cxfir32x32, 0,"cxfir32x32_bpf2.seq" },

  { 1, &descr_cxfir32x32ep, (tTestEngTarget)&api_cxfir32x32ep, 1,"cxfir32x32ep_lpf1.seq" },
  { 1, &descr_cxfir32x32ep, (tTestEngTarget)&api_cxfir32x32ep, 1,"cxfir32x32ep_bpf1.seq" },
  { 1, &descr_cxfir32x32ep, (tTestEngTarget)&api_cxfir32x32ep, 1,"cxfir32x32ep_4taps.seq"},
  { 1, &descr_cxfir32x32ep, (tTestEngTarget)&api_cxfir32x32ep, 0,"cxfir32x32ep_lpf2.seq" },
  { 1, &descr_cxfir32x32ep, (tTestEngTarget)&api_cxfir32x32ep, 0,"cxfir32x32ep_hpf1.seq" },
  { 1, &descr_cxfir32x32ep, (tTestEngTarget)&api_cxfir32x32ep, 0,"cxfir32x32ep_bpf2.seq" },

  /*
   * Stage 2
   */
   /* Block Real FIR Filter */
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 1,"bkfirf_lpf1.seq"},
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 1,"bkfirf_bpf1.seq"},
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 1,"bkfirf_8taps.seq"},
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 0,"bkfirf_lpf2.seq"},
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 0,"bkfirf_hpf1.seq"},
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 0,"bkfirf_bpf2.seq"},
  { 2, &descr_bkfirf, (tTestEngTarget)&api_bkfirf, 0,"bkfirf_4taps.seq"},

  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 1,"bkfiraf_lpf1.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 1,"bkfiraf_bpf1.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 1,"bkfiraf_3taps.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 0,"bkfiraf_lpf2.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 0,"bkfiraf_hpf1.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 0,"bkfiraf_bpf2.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 0,"bkfiraf_4taps.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 0,"bkfiraf_2taps.seq"},
  { 2, &descr_bkfiraf, (tTestEngTarget)&api_bkfiraf, 0,"bkfiraf_1tap.seq"},

  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 1,"stereo_bkfirf_lpf1.seq"},
  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 1,"stereo_bkfirf_bpf1.seq"},
  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 1,"stereo_bkfirf_tap8.seq"},
  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 0,"stereo_bkfirf_lpf2.seq"},
  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 0,"stereo_bkfirf_hpf1.seq"},
  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 0,"stereo_bkfirf_bpf2.seq"},
  { 2, &descr_stereo_bkfirf, (tTestEngTarget)&api_stereo_bkfirf, 0,"stereo_bkfirf_tap4.seq"},

  /* Block Complex FIR Filter */
  { 2, &descr_cxfirf, (tTestEngTarget)&api_cxfirf,1,"cxfirf_lpf1.seq"},
  { 2, &descr_cxfirf, (tTestEngTarget)&api_cxfirf,1,"cxfirf_bpf1.seq"},
  { 2, &descr_cxfirf, (tTestEngTarget)&api_cxfirf,1,"cxfirf_lpf2.seq"},
  { 2, &descr_cxfirf, (tTestEngTarget)&api_cxfirf,0,"cxfirf_hpf1.seq"},
  { 2, &descr_cxfirf, (tTestEngTarget)&api_cxfirf,0,"cxfirf_bpf2.seq"},
  { 2, &descr_cxfirf, (tTestEngTarget)&api_cxfirf,0,"cxfirf_4taps.seq"},
};

/* Perform all tests for FIR API functions. */
int main_firblk( int phaseNum, int isFull, int isVerbose, int breakOnError )
{
    int n;
    int res = 1;
    printf( "\nFIR (external IR):\n" );
    /* first, test FIR filters with external IR */
    for (n=0; n<(int)(sizeof(tests)/sizeof(tests[0])); n++)
    {
        if ( ( phaseNum == 0 || phaseNum == tests[n].phaseNum ) && ( isFull || tests[n].runAlways ) && (tests[n].pFirDescr->extraParam & TE_FIR_OLDEXTIR) )
        {
            tTestEngDesc desc;
            desc=*tests[n].pFirDescr;
            desc.extraParam|=TE_FIR_EXTIR;
            res &= (0!=TestEngRun(tests[n].fxns, &desc, tests[n].seqFile, isFull, isVerbose, breakOnError, 0));
            if (res == 0 && breakOnError) break;
        }
    }

    if (!res && breakOnError) return (res);

    printf( "\nFIR:\n" );
    for (n=0; n<(int)(sizeof(tests)/sizeof(tests[0])); n++)
    {
        if ( ( phaseNum == 0 || phaseNum == tests[n].phaseNum ) && ( isFull || tests[n].runAlways ) )
        {
            res &= (0!=TestEngRun(tests[n].fxns, tests[n].pFirDescr, tests[n].seqFile, isFull, isVerbose, breakOnError,0));
            if (res == 0 && breakOnError) break;
        }
    }

    return (res);
} /* main_firblk() */

