#include <criterion/criterion.h>
#include <stdlib.h>
#include <tc48/word.h>

static tc48_u128b to_unsigned_ref(const tc48_trit_state *trits, int bits)
{
    tc48_u128b res = 0;
    tc48_u128b pow = 1;

    for (int i = 0; i < bits; ++i) {
        res += (tc48_u128b) trits[i] * pow;
        pow *= 3;
    }
    return res;
}

static tc48_i128b to_signed_ref(const tc48_trit_state *trits, int bits, tc48_u128b values)
{
    tc48_u128b u = to_unsigned_ref(trits, bits);
    tc48_u128b limit = values / 2;

    if (u > limit)
        return (tc48_i128b) (u - values);

    return (tc48_i128b) u;
}

#define TEST_WORD(NAME, UNAME, TRITS, VALUES, SUFFIX)                        \
Test(word, NAME##_all) {                                                     \
    tc48_trit_state trits[TRITS];                                            \
                                                                             \
    for (int iter = 0; iter < 1000; ++iter) {                                \
        tc48_##NAME v = 0;                                                   \
                                                                             \
        /* set/get */                                                        \
        for (int i = 0; i < TRITS; ++i) {                                    \
            trits[i] = rand() % 3;                                           \
            tc48_##NAME##_set_trit(&v, i, trits[i]);                         \
            cr_assert_eq(tc48_##NAME##_get_trit(v, i), trits[i]);            \
        }                                                                    \
                                                                             \
        /* to_u / to_i */                                                    \
        cr_assert(tc48_##NAME##_to_u##SUFFIX(v) ==                           \
                  to_unsigned_ref(trits, TRITS));                            \
                                                                             \
        cr_assert(tc48_##NAME##_to_i##SUFFIX(v) ==                           \
                  to_signed_ref(trits, TRITS, VALUES));                      \
                                                                             \
        if (TRITS > 1) {                                                     \
            tc48_##NAME shl = tc48_##NAME##_shift(v, 1);                     \
                                                                             \
            cr_assert_eq(tc48_##NAME##_get_trit(shl, 0), TC48_ZERO);         \
            for (int i = 1; i < TRITS; ++i)                                  \
                cr_assert(tc48_##NAME##_get_trit(shl, i) == trits[i - 1]);   \
                                                                             \
            tc48_##NAME shr = tc48_##NAME##_shift(v, -1);                    \
                                                                             \
            cr_assert_eq(tc48_##NAME##_get_trit(shr, TRITS - 1), TC48_ZERO); \
            for (int i = 0; i < TRITS - 1; ++i)                              \
                cr_assert(tc48_##NAME##_get_trit(shr, i) == trits[i + 1]);   \
        }                                                                    \
                                                                             \
        /* overflow shift */                                                 \
        tc48_##NAME out = tc48_##NAME##_shift(v, TRITS);                     \
        cr_assert_eq(out, 0);                                                \
    }                                                                        \
}                                                                            \
Test(word, NAME##_large_shift) {                                             \
    tc48_##NAME v = 0;                                                       \
    tc48_##NAME##_set_trit(&v, TRITS - 1, 1);                                \
    for (int i = 1; i < TRITS; ++i) {                                        \
        tc48_##NAME shl = tc48_##NAME##_shift(v, i);                         \
        cr_assert_eq(shl, 0);                                                \
    }                                                                        \
}

TEST_WORD(doublet,    DOUBLET,    TC48_DOUBLET_TRITS,    TC48_DOUBLET_VALUES,    8b)
TEST_WORD(triplet,    TRIPLET,    TC48_TRIPLET_TRITS,    TC48_TRIPLET_VALUES,    8b)
TEST_WORD(quadruplet, QUADRUPLET, TC48_QUADRUPLET_TRITS, TC48_QUADRUPLET_VALUES, 8b)
TEST_WORD(tryte,      TRYTE,      TC48_TRYTE_TRITS,      TC48_TRYTE_VALUES,      16b)
TEST_WORD(quarter,    QUARTER,    TC48_QUARTER_TRITS,    TC48_QUARTER_VALUES,    32b)
TEST_WORD(half,       HALF,       TC48_HALF_TRITS,       TC48_HALF_VALUES,       64b)
TEST_WORD(word,       WORD,       TC48_WORD_TRITS,       TC48_WORD_VALUES,       128b)
