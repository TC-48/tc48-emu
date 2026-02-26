#pragma once

#include <stdint.h>
#include "tc48/trit.h"

typedef struct tc48_doublet    { uint8_t  l, h; } tc48_doublet;      /// 2 trits
typedef struct tc48_triplet    { uint8_t  l, h; } tc48_triplet;      /// 3 trits
typedef struct tc48_quadruplet { uint8_t  l, h; } tc48_quadruplet;   /// 4 trits
typedef struct tc48_tryte      { uint8_t  l, h; } tc48_tryte;        /// 6 trits
typedef struct tc48_quarter    { uint16_t l, h; } tc48_quarter;      /// 12 trits
typedef struct tc48_half       { uint32_t l, h; } tc48_half;         /// 24 trits
typedef struct tc48_word       { uint64_t l, h; } tc48_word;         /// 48 trits

typedef tc48_word tc48_addr;

/// Dual-Bit Representation: 1 bit per component (l/h) per trit
#define TC48_TRIT_BITS 1

#define TC48_DOUBLET_BITS    2
#define TC48_TRIPLET_BITS    3
#define TC48_QUADRUPLET_BITS 4
#define TC48_TRYTE_BITS      6
#define TC48_QUARTER_BITS    12
#define TC48_HALF_BITS       24
#define TC48_WORD_BITS       48

#define TC48_DOUBLET_MASK    0x3
#define TC48_TRIPLET_MASK    0x7
#define TC48_QUADRUPLET_MASK 0xF
#define TC48_TRYTE_MASK      0x3F
#define TC48_QUARTER_MASK    0xFFF
#define TC48_HALF_MASK       0xFFFFFF
#define TC48_WORD_MASK       0xFFFFFFFFFFFFULL

#define TC48_DOUBLET_VALUES    (9ULL)            /* 3^2  */
#define TC48_TRIPLET_VALUES    (27ULL)           /* 3^3  */
#define TC48_QUADRUPLET_VALUES (81ULL)           /* 3^4  */
#define TC48_TRYTE_VALUES      (729ULL)          /* 3^6  */
#define TC48_QUARTER_VALUES    (531441ULL)       /* 3^12 */
#define TC48_HALF_VALUES       (282429536481ULL) /* 3^24 */
#define TC48_WORD_VALUES       ((tc48_word){0x25C56DAFFABC35C1ULL, 0x10E4ULL}) /* 3^48 */

/* Literal helpers (trit-by-trit notation, MSB-first) */
#define _TC48_TL(t, n) (((uint64_t)(t) & 1ULL) << (n))
#define _TC48_TH(t, n) (((uint64_t)(t) >> 1ULL) << (n))

#define TC48_DOUBLET(t1, t0) ((tc48_doublet){       \
    (uint8_t)(_TC48_TL(t1, 1) | _TC48_TL(t0, 0)),   \
    (uint8_t)(_TC48_TH(t1, 1) | _TC48_TH(t0, 0))    \
})

#define TC48_TRIPLET(t2, t1, t0) ((tc48_triplet){                   \
    (uint8_t)(_TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)), \
    (uint8_t)(_TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0))  \
})

#define TC48_QUADRUPLET(t3, t2, t1, t0) ((tc48_quadruplet){                             \
    (uint8_t)(_TC48_TL(t3, 3) | _TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)),   \
    (uint8_t)(_TC48_TH(t3, 3) | _TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0))    \
})

#define TC48_TRYTE(t5, t4, t3, t2, t1, t0) ((tc48_tryte){                                                                   \
    (uint8_t)(_TC48_TL(t5, 5) | _TC48_TL(t4, 4) | _TC48_TL(t3, 3) | _TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)),   \
    (uint8_t)(_TC48_TH(t5, 5) | _TC48_TH(t4, 4) | _TC48_TH(t3, 3) | _TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0))    \
})

#define TC48_QUARTER(t11, t10, t9, t8, t7, t6, t5, t4, t3, t2, t1, t0) ((tc48_quarter){     \
    (uint16_t)(_TC48_TL(t11, 11) | _TC48_TL(t10, 10) | _TC48_TL(t9, 9) | _TC48_TL(t8, 8) |  \
               _TC48_TL(t7, 7) | _TC48_TL(t6, 6) | _TC48_TL(t5, 5) | _TC48_TL(t4, 4) |      \
               _TC48_TL(t3, 3) | _TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)),      \
    (uint16_t)(_TC48_TH(t11, 11) | _TC48_TH(t10, 10) | _TC48_TH(t9, 9) | _TC48_TH(t8, 8) |  \
               _TC48_TH(t7, 7) | _TC48_TH(t6, 6) | _TC48_TH(t5, 5) | _TC48_TH(t4, 4) |      \
               _TC48_TH(t3, 3) | _TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0))       \
})

#define TC48_GEN_WORD_UTILS(name, type, bits, mask)                                                 \
   static inline tc48_trit_state tc48_##name##_get_trit(tc48_##name w, int n) {                     \
       return (tc48_trit_state)(((w.l >> (unsigned)n) & 1U) | (((w.h >> (unsigned)n) & 1U) << 1U)); \
   }                                                                                                \
   static inline void tc48_##name##_set_trit(tc48_##name *w, int n, tc48_trit_state t) {            \
       w->l = (type)((w->l & ~((type)1 << (unsigned)n)) | (((type)t & 1U) << (unsigned)n));         \
       w->h = (type)((w->h & ~((type)1 << (unsigned)n)) | ((((type)t >> 1U) & 1U) << (unsigned)n)); \
   }                                                                                                \
   static inline tc48_##name tc48_##name##_shift(tc48_##name w, int count) {                        \
       tc48_##name res;                                                                             \
       if (count >= bits || count <= -bits) {                                                       \
           res.l = 0; res.h = 0;                                                                    \
       } else if (count >= 0) {                                                                     \
           res.l = (type)((w.l << (unsigned)count) & (mask));                                       \
           res.h = (type)((w.h << (unsigned)count) & (mask));                                       \
       } else {                                                                                     \
           res.l = (type)((w.l >> (unsigned)-count) & (mask));                                      \
           res.h = (type)((w.h >> (unsigned)-count) & (mask));                                      \
       }                                                                                            \
       return res;                                                                                  \
   }

TC48_GEN_WORD_UTILS(doublet,    uint8_t,   2,  TC48_DOUBLET_MASK)
TC48_GEN_WORD_UTILS(triplet,    uint8_t,   3,  TC48_TRIPLET_MASK)
TC48_GEN_WORD_UTILS(quadruplet, uint8_t,   4,  TC48_QUADRUPLET_MASK)
TC48_GEN_WORD_UTILS(tryte,      uint8_t,   6,  TC48_TRYTE_MASK)
TC48_GEN_WORD_UTILS(quarter,    uint16_t,  12, TC48_QUARTER_MASK)
TC48_GEN_WORD_UTILS(half,       uint32_t,  24, TC48_HALF_MASK)
TC48_GEN_WORD_UTILS(word,       uint64_t,  48, TC48_WORD_MASK)

#undef TC48_GEN_WORD_UTILS
