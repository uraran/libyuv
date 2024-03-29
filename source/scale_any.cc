/*
 *  Copyright 2015 The LibYuv Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS. All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "libyuv/scale.h"
#include "libyuv/scale_row.h"

#include "libyuv/basic_types.h"

#ifdef __cplusplus
namespace libyuv {
extern "C" {
#endif

// Definition for ScaleFilterCols, ScaleARGBCols and ScaleARGBFilterCols
#define CANY(NAMEANY, TERP_SIMD, TERP_C, BPP, MASK)                            \
    void NAMEANY(uint8* dst_ptr, const uint8* src_ptr,                         \
                 int dst_width, int x, int dx) {                               \
      int n = dst_width & ~MASK;                                               \
      if (n > 0) {                                                             \
        TERP_SIMD(dst_ptr, src_ptr, n, x, dx);                                 \
      }                                                                        \
      TERP_C(dst_ptr + n * BPP, src_ptr,                                       \
             dst_width & MASK, x + n * dx, dx);                                \
    }

#ifdef HAS_SCALEFILTERCOLS_NEON
CANY(ScaleFilterCols_Any_NEON, ScaleFilterCols_NEON, ScaleFilterCols_C, 1, 7)
#endif
#ifdef HAS_SCALEARGBCOLS_NEON
CANY(ScaleARGBCols_Any_NEON, ScaleARGBCols_NEON, ScaleARGBCols_C, 4, 7)
#endif
#undef CANY

// Fixed scale down.
#define SDANY(NAMEANY, SCALEROWDOWN_SIMD, SCALEROWDOWN_C, FACTOR, BPP, MASK)   \
    void NAMEANY(const uint8* src_ptr, ptrdiff_t src_stride,                   \
                 uint8* dst_ptr, int dst_width) {                              \
      int n = dst_width & ~MASK;                                               \
      if (n > 0) {                                                             \
        SCALEROWDOWN_SIMD(src_ptr, src_stride, dst_ptr, n);                    \
      }                                                                        \
      SCALEROWDOWN_C(src_ptr + (n * FACTOR) * BPP, src_stride,                 \
                     dst_ptr + n * BPP, dst_width & MASK);                     \
    }


#ifdef HAS_SCALEROWDOWN2_SSE2
SDANY(ScaleRowDown2_Any_SSE2, ScaleRowDown2_SSE2, ScaleRowDown2_C, 2, 1, 15)
SDANY(ScaleRowDown2Linear_Any_SSE2, ScaleRowDown2Linear_SSE2,
      ScaleRowDown2Linear_C, 2, 1, 15)
SDANY(ScaleRowDown2Box_Any_SSE2, ScaleRowDown2Box_SSE2, ScaleRowDown2Box_C,
      2, 1, 15)
#endif
#ifdef HAS_SCALEROWDOWN2_AVX2
SDANY(ScaleRowDown2_Any_AVX2, ScaleRowDown2_AVX2, ScaleRowDown2_C, 2, 1, 31)
SDANY(ScaleRowDown2Linear_Any_AVX2, ScaleRowDown2Linear_AVX2,
      ScaleRowDown2Linear_C, 2, 1, 31)
SDANY(ScaleRowDown2Box_Any_AVX2, ScaleRowDown2Box_AVX2, ScaleRowDown2Box_C,
      2, 1, 31)
#endif
#ifdef HAS_SCALEROWDOWN2_NEON
SDANY(ScaleRowDown2_Any_NEON, ScaleRowDown2_NEON, ScaleRowDown2_C, 2, 1, 15)
SDANY(ScaleRowDown2Linear_Any_NEON, ScaleRowDown2Linear_NEON,
      ScaleRowDown2Linear_C, 2, 1, 15)
SDANY(ScaleRowDown2Box_Any_NEON, ScaleRowDown2Box_NEON,
      ScaleRowDown2Box_C, 2, 1, 15)
#endif
#ifdef HAS_SCALEROWDOWN4_SSE2
SDANY(ScaleRowDown4_Any_SSE2, ScaleRowDown4_SSE2, ScaleRowDown4_C, 4, 1, 7)
SDANY(ScaleRowDown4Box_Any_SSE2, ScaleRowDown4Box_SSE2, ScaleRowDown4Box_C,
      4, 1, 7)
#endif
#ifdef HAS_SCALEROWDOWN4_NEON
SDANY(ScaleRowDown4_Any_NEON, ScaleRowDown4_NEON, ScaleRowDown4_C, 4, 1, 7)
SDANY(ScaleRowDown4Box_Any_NEON, ScaleRowDown4Box_NEON, ScaleRowDown4Box_C,
      4, 1, 7)
#endif
#undef SDANY

#ifdef __cplusplus
}  // extern "C"
}  // namespace libyuv
#endif

