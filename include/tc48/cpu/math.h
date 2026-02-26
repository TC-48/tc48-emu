#pragma once

#include <tc48/word.h>
#include <stdbool.h>

#define TC48_ALU_OP_DECL(name) \
    tc48_##name tc48_math_##name##_add(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_sub(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_mul(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_div(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_mod(tc48_##name a, tc48_##name b); \
    \
    tc48_##name tc48_math_##name##_sadd(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_ssub(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_smul(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_sdiv(tc48_##name a, tc48_##name b); \
    tc48_##name tc48_math_##name##_smod(tc48_##name a, tc48_##name b);

TC48_ALU_OP_DECL(doublet)
TC48_ALU_OP_DECL(triplet)
TC48_ALU_OP_DECL(quadruplet)
TC48_ALU_OP_DECL(tryte)
TC48_ALU_OP_DECL(quarter)
TC48_ALU_OP_DECL(half)
TC48_ALU_OP_DECL(word)

#undef TC48_ALU_OP_DECL
