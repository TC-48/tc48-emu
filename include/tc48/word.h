#pragma once

#include <stdint.h>

#include <tc48/trit.h>
#include <tc48/defs.h>

typedef struct tc48_doublet    { tc48_u8b  l, h; } tc48_doublet;      /// 2 trits
typedef struct tc48_triplet    { tc48_u8b  l, h; } tc48_triplet;      /// 3 trits
typedef struct tc48_quadruplet { tc48_u8b  l, h; } tc48_quadruplet;   /// 4 trits
typedef struct tc48_tryte      { tc48_u8b  l, h; } tc48_tryte;        /// 6 trits
typedef struct tc48_quarter    { tc48_u16b l, h; } tc48_quarter;      /// 12 trits
typedef struct tc48_half       { tc48_u32b l, h; } tc48_half;         /// 24 trits
typedef struct tc48_word       { tc48_u64b l, h; } tc48_word;         /// 48 trits

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

#define TC48_DOUBLET_VALUES    (3ULL * 3ULL)                                     /* 3^2  */
#define TC48_TRIPLET_VALUES    (TC48_DOUBLET_VALUES * 3ULL)                      /* 3^3  */
#define TC48_QUADRUPLET_VALUES (TC48_TRIPLET_VALUES * 3ULL)                      /* 3^4  */
#define TC48_TRYTE_VALUES      (TC48_QUADRUPLET_VALUES * 9ULL)                   /* 3^6  */
#define TC48_QUARTER_VALUES    (TC48_TRYTE_VALUES * TC48_TRYTE_VALUES)           /* 3^12 */
#define TC48_HALF_VALUES       (TC48_QUARTER_VALUES * TC48_QUARTER_VALUES)       /* 3^24 */
#define TC48_WORD_VALUES       ((tc48_u128b)TC48_HALF_VALUES * TC48_HALF_VALUES) /* 3^48 */

// Literal helpers (trit-by-trit notation, MSB-first)
#define _TC48_TL(t, n) (((tc48_u64b)(t) & 1ULL) << (n))
#define _TC48_TH(t, n) (((tc48_u64b)(t) >> 1ULL) << (n))

#include <tc48/gen/word-lits.h>

#define TC48_GEN_WORD_UTILS(NAME, TYPE, BITS, MASK)                                                     \
   static inline tc48_trit_state tc48_##NAME##_get_trit(tc48_##NAME w, int n) {                         \
       return (tc48_trit_state)(((w.l >> (unsigned) n) & 1U) | (((w.h >> (unsigned) n) & 1U) << 1U));   \
   }                                                                                                    \
   static inline void tc48_##NAME##_set_trit(tc48_##NAME *w, int n, tc48_trit_state t) {                \
       w->l = (TYPE)((w->l & ~((TYPE)1 << (unsigned) n)) | (((TYPE)t & 1U) << (unsigned) n));           \
       w->h = (TYPE)((w->h & ~((TYPE)1 << (unsigned) n)) | ((((TYPE)t >> 1U) & 1U) << (unsigned) n));   \
   }                                                                                                    \
   static inline tc48_##NAME tc48_##NAME##_shift(tc48_##NAME w, int count) {                            \
       tc48_##NAME res;                                                                                 \
       if (count >= BITS || count <= -BITS) {                                                           \
           res.l = 0; res.h = 0;                                                                        \
       } else if (count >= 0) {                                                                         \
           res.l = (TYPE) ((w.l << (unsigned) count) & (MASK));                                         \
           res.h = (TYPE) ((w.h << (unsigned) count) & (MASK));                                         \
       } else {                                                                                         \
           res.l = (TYPE) ((w.l >> (unsigned) -count) & (MASK));                                        \
           res.h = (TYPE) ((w.h >> (unsigned) -count) & (MASK));                                        \
       }                                                                                                \
       return res;                                                                                      \
   }

TC48_GEN_WORD_UTILS(doublet,    tc48_u8b,   2,  TC48_DOUBLET_MASK)
TC48_GEN_WORD_UTILS(triplet,    tc48_u8b,   3,  TC48_TRIPLET_MASK)
TC48_GEN_WORD_UTILS(quadruplet, tc48_u8b,   4,  TC48_QUADRUPLET_MASK)
TC48_GEN_WORD_UTILS(tryte,      tc48_u8b,   6,  TC48_TRYTE_MASK)
TC48_GEN_WORD_UTILS(quarter,    tc48_u16b,  12, TC48_QUARTER_MASK)
TC48_GEN_WORD_UTILS(half,       tc48_u32b,  24, TC48_HALF_MASK)
TC48_GEN_WORD_UTILS(word,       tc48_u64b,  48, TC48_WORD_MASK)

#undef TC48_GEN_WORD_UTILS
