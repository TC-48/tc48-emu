#pragma once

#include <stdint.h>

#include <tc48/trit.h>
#include <tc48/defs.h>
#include <tc48/pow3.h>

typedef tc48_u8b   tc48_doublet;      /// 2 trits
typedef tc48_u8b   tc48_triplet;      /// 3 trits
typedef tc48_u8b   tc48_quadruplet;   /// 4 trits
typedef tc48_u16b  tc48_tryte;        /// 6 trits
typedef tc48_u32b  tc48_quarter;      /// 12 trits
typedef tc48_u64b  tc48_half;         /// 24 trits
typedef tc48_u128b tc48_word;         /// 48 trits

#define TC48_DOUBLET_TRITS    2
#define TC48_TRIPLET_TRITS    3
#define TC48_QUADRUPLET_TRITS 4
#define TC48_TRYTE_TRITS      6
#define TC48_QUARTER_TRITS    12
#define TC48_HALF_TRITS       24
#define TC48_WORD_TRITS       48

#define TC48_DOUBLET_VALUES    (3ULL * 3ULL)                                     /// 3^2
#define TC48_TRIPLET_VALUES    (TC48_DOUBLET_VALUES * 3ULL)                      /// 3^3
#define TC48_QUADRUPLET_VALUES (TC48_TRIPLET_VALUES * 3ULL)                      /// 3^4
#define TC48_TRYTE_VALUES      (TC48_QUADRUPLET_VALUES * 9ULL)                   /// 3^6
#define TC48_QUARTER_VALUES    (TC48_TRYTE_VALUES * TC48_TRYTE_VALUES)           /// 3^12
#define TC48_HALF_VALUES       (TC48_QUARTER_VALUES * TC48_QUARTER_VALUES)       /// 3^24
#define TC48_WORD_VALUES       ((tc48_u128b)TC48_HALF_VALUES * TC48_HALF_VALUES) /// 3^48

#define _TC48_T(t, n) ((tc48_u128b)(t) * tc48_pow3_u128[n])

#include <tc48/gen/word-lits.h>

#define TC48_GEN_WORD_UTILS(NAME, TYPE, TRITS, VALUES, U_BIN, I_BIN, SUFFIX, POW_TABLE)                  \
   static inline tc48_trit_state tc48_##NAME##_get_trit(tc48_##NAME w, int n) {                          \
       return (tc48_trit_state)((w / POW_TABLE[TRITS - 1 - n]) % 3);                                     \
   }                                                                                                     \
   static inline void tc48_##NAME##_set_trit(tc48_##NAME *w, int n, tc48_trit_state t) {                 \
       tc48_u128b p = POW_TABLE[TRITS - 1 - n];                                                          \
       *w = (TYPE)(*w - (tc48_##NAME##_get_trit(*w, n) * p) + ((TYPE)t * p));                             \
   }                                                                                                     \
   static inline tc48_##NAME tc48_##NAME##_shift(tc48_##NAME w, int count) {                             \
       if (count >= TRITS || count <= -TRITS) return 0;                                                  \
       if (count >= 0) return (tc48_##NAME)(((TYPE)w % (VALUES / POW_TABLE[count])) * POW_TABLE[count]); \
       return (tc48_##NAME)(w / POW_TABLE[-count]);                                                      \
   }                                                                                                     \
   static inline U_BIN tc48_##NAME##_to_u##SUFFIX(tc48_##NAME w) {                                       \
       return (U_BIN)w;                                                                                  \
   }                                                                                                     \
   static inline I_BIN tc48_##NAME##_to_i##SUFFIX(tc48_##NAME w) {                                       \
       U_BIN u = (U_BIN)w;                                                                               \
       U_BIN limit = (U_BIN)((VALUES) / 2);                                                              \
       if (u > limit) return (I_BIN)(u - (U_BIN)(VALUES));                                               \
       return (I_BIN)u;                                                                                  \
   }

TC48_GEN_WORD_UTILS(doublet,    tc48_u8b,   2,  TC48_DOUBLET_VALUES,    tc48_u8b,   tc48_i8b,   8b,   tc48_pow3_u32)
TC48_GEN_WORD_UTILS(triplet,    tc48_u8b,   3,  TC48_TRIPLET_VALUES,    tc48_u8b,   tc48_i8b,   8b,   tc48_pow3_u32)
TC48_GEN_WORD_UTILS(quadruplet, tc48_u8b,   4,  TC48_QUADRUPLET_VALUES, tc48_u8b,   tc48_i8b,   8b,   tc48_pow3_u32)
TC48_GEN_WORD_UTILS(tryte,      tc48_u16b,  6,  TC48_TRYTE_VALUES,      tc48_u16b,  tc48_i16b,  16b,  tc48_pow3_u32)
TC48_GEN_WORD_UTILS(quarter,    tc48_u32b,  12, TC48_QUARTER_VALUES,    tc48_u32b,  tc48_i32b,  32b,  tc48_pow3_u32)
TC48_GEN_WORD_UTILS(half,       tc48_u64b,  24, TC48_HALF_VALUES,       tc48_u64b,  tc48_i64b,  64b,  tc48_pow3_u64)
TC48_GEN_WORD_UTILS(word,       tc48_u128b, 48, TC48_WORD_VALUES,       tc48_u128b, tc48_i128b, 128b, tc48_pow3_u128)

#undef TC48_GEN_WORD_UTILS
