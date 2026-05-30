#pragma once

#include <tc48/word.h>
#include <stdbool.h>

#define TC48_MATH_OP_DECL(name)                                        \
    tc48_##name tc48_math_##name##_min(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_max(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_rot(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_not(tc48_##name a);                 \
    tc48_##name tc48_math_##name##_neg(tc48_##name a);                 \
    tc48_##name tc48_math_##name##_shl(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_shr(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_add(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_sub(tc48_##name a, tc48_##name b);  \
    tc48_##name tc48_math_##name##_umul(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_udiv(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_smul(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_sdiv(tc48_##name a, tc48_##name b); \

TC48_MATH_OP_DECL(doublet);
TC48_MATH_OP_DECL(triplet);
TC48_MATH_OP_DECL(quadruplet);
TC48_MATH_OP_DECL(tryte);
TC48_MATH_OP_DECL(quarter);
TC48_MATH_OP_DECL(half);
TC48_MATH_OP_DECL(word);

#undef TC48_MATH_OP_DECL
