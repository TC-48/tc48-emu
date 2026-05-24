#pragma once

#include <tc48/word.h>
#include <stdbool.h>

#define TC48_MATH_OP_DECL(name)                                       \
    tc48_##name tc48_math_##name##_and(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_or (tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_xor(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_not(tc48_##name a);                \
    tc48_##name tc48_math_##name##_shl(tc48_##name a, int count);     \
    tc48_##name tc48_math_##name##_shr(tc48_##name a, int count);

TC48_MATH_OP_DECL(doublet);
TC48_MATH_OP_DECL(triplet);
TC48_MATH_OP_DECL(quadruplet);
TC48_MATH_OP_DECL(tryte);
TC48_MATH_OP_DECL(quarter);
TC48_MATH_OP_DECL(half);
TC48_MATH_OP_DECL(word);

#undef TC48_MATH_OP_DECL
