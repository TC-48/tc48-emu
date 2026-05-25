#include <tc48/cpu/math.h>
#include <tc48/pow3.h>

#define TC48_MIN(a, b) ((a) < (b) ? (a) : (b))
#define TC48_MAX(a, b) ((a) > (b) ? (a) : (b))

#define TC48_MATH_IMPL(NAME, TRITS, VALUES, POW_TABLE)                 \
    tc48_##NAME tc48_math_##NAME##_min(tc48_##NAME a, tc48_##NAME b) { \
        tc48_##NAME res = 0;                                           \
        for (tc48_u8b i = 0; i < TRITS; i++) {                         \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;               \
            tc48_trit_state tb = (b / POW_TABLE[i]) % 3;               \
            res += (tc48_##NAME)TC48_MIN(ta, tb) * POW_TABLE[i];       \
        }                                                              \
        return res;                                                    \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_max(tc48_##NAME a, tc48_##NAME b) { \
        tc48_##NAME res = 0;                                           \
        for (tc48_u8b i = 0; i < TRITS; i++) {                         \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;               \
            tc48_trit_state tb = (b / POW_TABLE[i]) % 3;               \
            res += (tc48_##NAME)TC48_MAX(ta, tb) * POW_TABLE[i];       \
        }                                                              \
        return res;                                                    \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_rot(tc48_##NAME a, tc48_##NAME b) { \
        tc48_##NAME res = 0;                                           \
        for (tc48_u8b i = 0; i < TRITS; i++) {                         \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;               \
            tc48_trit_state tb = (b / POW_TABLE[i]) % 3;               \
            res += (tc48_##NAME)((ta + tb) % 3) * POW_TABLE[i];        \
        }                                                              \
        return res;                                                    \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_not(tc48_##NAME a) {                \
        tc48_##NAME res = 0;                                           \
        for (tc48_u8b i = 0; i < TRITS; i++) {                         \
            tc48_trit_state ta = (a / POW_TABLE[i]) % 3;               \
            res += (tc48_##NAME)(2 - ta) * POW_TABLE[i];               \
        }                                                              \
        return res;                                                    \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_shl(tc48_##NAME a, int count) {     \
        return tc48_##NAME##_shift(a, count);                          \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_shr(tc48_##NAME a, int count) {     \
        return tc48_##NAME##_shift(a, -count);                         \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_add(tc48_##NAME a, tc48_##NAME b) { \
        return (tc48_##NAME)(((tc48_u128b)a + b) % VALUES);            \
    }                                                                  \
    tc48_##NAME tc48_math_##NAME##_sub(tc48_##NAME a, tc48_##NAME b) { \
        return (tc48_##NAME)(((tc48_u128b)a                            \
                    + VALUES - (b % VALUES)) % VALUES);                \
    }

TC48_MATH_IMPL(doublet,    2,  TC48_DOUBLET_VALUES,    tc48_pow3_u32);
TC48_MATH_IMPL(triplet,    3,  TC48_TRIPLET_VALUES,    tc48_pow3_u32);
TC48_MATH_IMPL(quadruplet, 4,  TC48_QUADRUPLET_VALUES, tc48_pow3_u32);
TC48_MATH_IMPL(tryte,      6,  TC48_TRYTE_VALUES,      tc48_pow3_u32);
TC48_MATH_IMPL(quarter,    12, TC48_QUARTER_VALUES,    tc48_pow3_u32);
TC48_MATH_IMPL(half,       24, TC48_HALF_VALUES,       tc48_pow3_u64);
TC48_MATH_IMPL(word,       48, TC48_WORD_VALUES,       tc48_pow3_u128);
