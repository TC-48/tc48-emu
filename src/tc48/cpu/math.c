#include <tc48/cpu/math.h>
#include <tc48/pow3.h>

#define TC48_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TC48_MAX(a, b) ((a) > (b) ? (a) : (b))

#define TC48_MATH_IMPL(NAME, TRITS, VALUES, POW_TABLE, SUFFIX)          \
    tc48_##NAME tc48_math_##NAME##_min(tc48_##NAME a, tc48_##NAME b) {  \
        tc48_##NAME res = 0;                                            \
        for (tc48_u8b i = 0; i < TRITS; i++) {                          \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;                \
            tc48_trit_state tb = (b / POW_TABLE[i]) % 3;                \
            res += (tc48_##NAME)TC48_MIN(ta, tb) * POW_TABLE[i];        \
        }                                                               \
        return res;                                                     \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_max(tc48_##NAME a, tc48_##NAME b) {  \
        tc48_##NAME res = 0;                                            \
        for (tc48_u8b i = 0; i < TRITS; i++) {                          \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;                \
            tc48_trit_state tb = (b / POW_TABLE[i]) % 3;                \
            res += (tc48_##NAME)TC48_MAX(ta, tb) * POW_TABLE[i];        \
        }                                                               \
        return res;                                                     \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_rot(tc48_##NAME a, tc48_##NAME b) {  \
        tc48_##NAME res = 0;                                            \
        for (tc48_u8b i = 0; i < TRITS; i++) {                          \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;                \
            tc48_trit_state tb = (b / POW_TABLE[i]) % 3;                \
            res += (tc48_##NAME)((ta + tb) % 3) * POW_TABLE[i];         \
        }                                                               \
        return res;                                                     \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_not(tc48_##NAME a) {                 \
        tc48_##NAME res = 0;                                            \
        for (tc48_u8b i = 0; i < TRITS; i++) {                          \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;                \
            res += (tc48_##NAME)(2 - ta) * POW_TABLE[i];                \
        }                                                               \
        return res;                                                     \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_shl(tc48_##NAME a, int count) {      \
        return tc48_##NAME##_shift(a, count);                           \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_shr(tc48_##NAME a, int count) {      \
        return tc48_##NAME##_shift(a, -count);                          \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_add(tc48_##NAME a, tc48_##NAME b) {  \
        return (tc48_##NAME)(((tc48_u128b)a + b) % VALUES);             \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_sub(tc48_##NAME a, tc48_##NAME b) {  \
        return (tc48_##NAME)(((tc48_u128b)a                             \
                    + VALUES - (b % VALUES)) % VALUES);                 \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_umul(tc48_##NAME a, tc48_##NAME b) { \
        if (TRITS <= 24) {                                              \
            return (tc48_##NAME)(((tc48_u128b)a * b) % VALUES);         \
        } else {                                                        \
            tc48_u128b split = tc48_pow3_u128[TRITS/2];                 \
            tc48_##NAME a0 = a % split;                                 \
            tc48_##NAME a1 = a / split;                                 \
            tc48_##NAME b0 = b % split;                                 \
            tc48_##NAME b1 = b / split;                                 \
            tc48_u128b res = ((tc48_u128b)a1*b0 + (tc48_u128b)a0 * b1)  \
                * split + (tc48_u128b)a0 * b0;                          \
            return (tc48_##NAME)(res % VALUES);                         \
        }                                                               \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_udiv(tc48_##NAME a, tc48_##NAME b) { \
        if (b == 0) return 0;                                           \
        return (tc48_##NAME)(a / b);                                    \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_smul(tc48_##NAME a, tc48_##NAME b) { \
        return tc48_math_##NAME##_umul(a, b);                           \
    }                                                                   \
    tc48_##NAME tc48_math_##NAME##_sdiv(tc48_##NAME a, tc48_##NAME b) { \
        if (b == 0) return 0;                                           \
        tc48_i128b sa = tc48_##NAME##_to_i##SUFFIX(a);                  \
        tc48_i128b sb = tc48_##NAME##_to_i##SUFFIX(b);                  \
        tc48_i128b res = sa / sb;                                       \
        tc48_i128b m = (tc48_i128b)VALUES;                              \
        return (tc48_##NAME)((res % m + m) % m);                        \
    }

TC48_MATH_IMPL(doublet,    2,  TC48_DOUBLET_VALUES,    tc48_pow3_u32,  8b);
TC48_MATH_IMPL(triplet,    3,  TC48_TRIPLET_VALUES,    tc48_pow3_u32,  8b);
TC48_MATH_IMPL(quadruplet, 4,  TC48_QUADRUPLET_VALUES, tc48_pow3_u32,  8b);
TC48_MATH_IMPL(tryte,      6,  TC48_TRYTE_VALUES,      tc48_pow3_u32,  16b);
TC48_MATH_IMPL(quarter,    12, TC48_QUARTER_VALUES,    tc48_pow3_u32,  32b);
TC48_MATH_IMPL(half,       24, TC48_HALF_VALUES,       tc48_pow3_u64,  64b);
TC48_MATH_IMPL(word,       48, TC48_WORD_VALUES,       tc48_pow3_u128, 128b);
