#pragma once

#include <stdint.h>

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

#define TC48_DOUBLET(t1, t0) ((tc48_doublet){ \
    (uint8_t)(_TC48_TL(t1, 1) | _TC48_TL(t0, 0)), \
    (uint8_t)(_TC48_TH(t1, 1) | _TC48_TH(t0, 0)) \
})

#define TC48_TRIPLET(t2, t1, t0) ((tc48_triplet){ \
    (uint8_t)(_TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)), \
    (uint8_t)(_TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0)) \
})

#define TC48_QUADRUPLET(t3, t2, t1, t0) ((tc48_quadruplet){ \
    (uint8_t)(_TC48_TL(t3, 3) | _TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)), \
    (uint8_t)(_TC48_TH(t3, 3) | _TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0)) \
})

#define TC48_TRYTE(t5, t4, t3, t2, t1, t0) ((tc48_tryte){ \
    (uint8_t)(_TC48_TL(t5, 5) | _TC48_TL(t4, 4) | _TC48_TL(t3, 3) | _TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)), \
    (uint8_t)(_TC48_TH(t5, 5) | _TC48_TH(t4, 4) | _TC48_TH(t3, 3) | _TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0)) \
})

#define TC48_QUARTER(t11, t10, t9, t8, t7, t6, t5, t4, t3, t2, t1, t0) ((tc48_quarter){ \
    (uint16_t)(_TC48_TL(t11, 11) | _TC48_TL(t10, 10) | _TC48_TL(t9, 9) | _TC48_TL(t8, 8) | \
               _TC48_TL(t7, 7) | _TC48_TL(t6, 6) | _TC48_TL(t5, 5) | _TC48_TL(t4, 4) | \
               _TC48_TL(t3, 3) | _TC48_TL(t2, 2) | _TC48_TL(t1, 1) | _TC48_TL(t0, 0)), \
    (uint16_t)(_TC48_TH(t11, 11) | _TC48_TH(t10, 10) | _TC48_TH(t9, 9) | _TC48_TH(t8, 8) | \
               _TC48_TH(t7, 7) | _TC48_TH(t6, 6) | _TC48_TH(t5, 5) | _TC48_TH(t4, 4) | \
               _TC48_TH(t3, 3) | _TC48_TH(t2, 2) | _TC48_TH(t1, 1) | _TC48_TH(t0, 0)) \
})
