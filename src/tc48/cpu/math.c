#include <tc48/cpu/math.h>
#include <tc48/pow3.h>

#include <stdint.h>

// up to 12 trits (quarter word) fit in tc48_u32b
static inline tc48_u32b tc48_to_u32(tc48_u64b l, tc48_u64b h, int bits) {
    tc48_u32b res = 0;
    for (int i = 0; i < bits; i++) {
        tc48_u8b t = ((l >> i) & 1) | (((h >> i) & 1) << 1);
        if (t)
            res += (tc48_u32b)t * tc48_pow3_u32[i];
    }
    return res;
}
static inline void tc48_from_u32(tc48_u64b* l, tc48_u64b* h, tc48_u32b val, int bits) {
    *l = 0;
    *h = 0;
    for (int i = 0; i < bits; i++) {
        tc48_u8b t = val % 3;
        val /= 3;
        if (t & 1) {
            *l |= (1ULL << i);
        }
        if (t & 2) {
            *h |= (1ULL << i);
        }
    }
}

// up to 24 trits (half word) fit in tc48_u64b
static inline tc48_u64b tc48_to_u64(tc48_u64b l, tc48_u64b h, int bits) {
    tc48_u64b res = 0;
    for (int i = 0; i < bits; i++) {
        tc48_u8b t = ((l >> i) & 1) | (((h >> i) & 1) << 1);
        if (t)
            res += (tc48_u64b)t * tc48_pow3_u64[i];
    }
    return res;
}
static inline void tc48_from_u64(tc48_u64b* l, tc48_u64b* h, tc48_u64b val, int bits) {
    *l = 0;
    *h = 0;
    for (int i = 0; i < bits; i++) {
        tc48_u8b t = val % 3;
        val /= 3;
        if (t & 1)
            *l |= (1ULL << i);
        if (t & 2)
            *h |= (1ULL << i);
    }
}

// up to 48 trits (word) fit in __uint128_t
static inline tc48_u128b tc48_to_u128(tc48_u64b l, tc48_u64b h, int bits) {
    tc48_u128b res = 0;
    for (int i = 0; i < bits; i++) {
        tc48_u8b t = ((l >> i) & 1) | (((h >> i) & 1) << 1);
        if (t) {
            res += (tc48_u128b)t * tc48_pow3_u128[i];
        }
    }
    return res;
}
static inline void tc48_from_u128(tc48_u64b* l, tc48_u64b* h, tc48_u128b val, int bits) {
    *l = 0;
    *h = 0;
    for (int i = 0; i < bits; i++) {
        tc48_u8b t = val % 3;
        val /= 3;
        if (t & 1) {
            *l |= (1ULL << i);
        }
        if (t & 2) {
            *h |= (1ULL << i);
        }
    }
}

#define TC48_ALU_IMPL(NAME, TYPE, BITS, U_NATIVE, S_NATIVE, POW_ARR, TO_FN, FROM_FN) \
    tc48_##NAME tc48_math_##NAME##_add(tc48_##NAME a, tc48_##NAME b) {               \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        U_NATIVE vr = (va + vb) % m;                                                 \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_sub(tc48_##NAME a, tc48_##NAME b) {               \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        U_NATIVE vr = (va + m - (vb % m)) % m;                                       \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_mul(tc48_##NAME a, tc48_##NAME b) {               \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        U_NATIVE vr = (va * vb) % m;                                                 \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_div(tc48_##NAME a, tc48_##NAME b) {               \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        if (vb == 0)                                                                 \
          return (tc48_##NAME){0, 0};                                                \
        U_NATIVE vr = (va / vb);                                                     \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_mod(tc48_##NAME a, tc48_##NAME b) {               \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        if (vb == 0)                                                                 \
          return (tc48_##NAME){0, 0};                                                \
        U_NATIVE vr = (va % vb);                                                     \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
                                                                                     \
    /* Signed Operations */                                                          \
    static inline S_NATIVE tc48_math_##NAME##_to_signed(U_NATIVE v, U_NATIVE m) {    \
        U_NATIVE half = m / 2;                                                       \
        return (v > half) ? (S_NATIVE)v - (S_NATIVE)m : (S_NATIVE)v;                 \
    }                                                                                \
    static inline U_NATIVE tc48_math_##NAME##_from_signed(S_NATIVE v, U_NATIVE m) {  \
        U_NATIVE half = m / 2;                                                       \
        while (v > (S_NATIVE)half)                                                   \
          v -= (S_NATIVE)m;                                                          \
        while (v < -(S_NATIVE)half)                                                  \
          v += (S_NATIVE)m;                                                          \
        return (v < 0) ? (U_NATIVE)(v + (S_NATIVE)m) : (U_NATIVE)v;                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_sadd(tc48_##NAME a, tc48_##NAME b) {              \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        S_NATIVE sa = tc48_math_##NAME##_to_signed(va, m);                           \
        S_NATIVE sb = tc48_math_##NAME##_to_signed(vb, m);                           \
        U_NATIVE vr = tc48_math_##NAME##_from_signed(sa + sb, m);                    \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_ssub(tc48_##NAME a, tc48_##NAME b) {              \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        S_NATIVE sa = tc48_math_##NAME##_to_signed(va, m);                           \
        S_NATIVE sb = tc48_math_##NAME##_to_signed(vb, m);                           \
        U_NATIVE vr = tc48_math_##NAME##_from_signed(sa - sb, m);                    \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_smul(tc48_##NAME a, tc48_##NAME b) {              \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        S_NATIVE sa = tc48_math_##NAME##_to_signed(va, m);                           \
        S_NATIVE sb = tc48_math_##NAME##_to_signed(vb, m);                           \
        U_NATIVE vr = tc48_math_##NAME##_from_signed(sa * sb, m);                    \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_sdiv(tc48_##NAME a, tc48_##NAME b) {              \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        S_NATIVE sa = tc48_math_##NAME##_to_signed(va, m);                           \
        S_NATIVE sb = tc48_math_##NAME##_to_signed(vb, m);                           \
        if (sb == 0)                                                                 \
          return (tc48_##NAME) { 0, 0 };                                             \
        U_NATIVE vr = tc48_math_##NAME##_from_signed(sa / sb, m);                    \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }                                                                                \
    tc48_##NAME tc48_math_##NAME##_smod(tc48_##NAME a, tc48_##NAME b) {              \
        U_NATIVE va = TO_FN(a.l, a.h, BITS);                                         \
        U_NATIVE vb = TO_FN(b.l, b.h, BITS);                                         \
        U_NATIVE m = POW_ARR[BITS];                                                  \
        S_NATIVE sa = tc48_math_##NAME##_to_signed(va, m);                           \
        S_NATIVE sb = tc48_math_##NAME##_to_signed(vb, m);                           \
        if (sb == 0)                                                                 \
          return (tc48_##NAME) { 0, 0 };                                             \
        U_NATIVE vr = tc48_math_##NAME##_from_signed(sa % sb, m);                    \
        tc48_##NAME res;                                                             \
        tc48_u64b rl, rh;                                                            \
        FROM_FN(&rl, &rh, vr, BITS);                                                 \
        res.l = (TYPE)rl;                                                            \
        res.h = (TYPE)rh;                                                            \
        return res;                                                                  \
    }

TC48_ALU_IMPL(doublet,    tc48_u8b,  2,  tc48_u32b,  tc48_i32b,  tc48_pow3_u32,  tc48_to_u32,  tc48_from_u32)
TC48_ALU_IMPL(triplet,    tc48_u8b,  3,  tc48_u32b,  tc48_i32b,  tc48_pow3_u32,  tc48_to_u32,  tc48_from_u32)
TC48_ALU_IMPL(quadruplet, tc48_u8b,  4,  tc48_u32b,  tc48_i32b,  tc48_pow3_u32,  tc48_to_u32,  tc48_from_u32)
TC48_ALU_IMPL(tryte,      tc48_u8b,  6,  tc48_u32b,  tc48_i32b,  tc48_pow3_u32,  tc48_to_u32,  tc48_from_u32)
TC48_ALU_IMPL(quarter,    tc48_u16b, 12, tc48_u32b,  tc48_i32b,  tc48_pow3_u32,  tc48_to_u32,  tc48_from_u32)
TC48_ALU_IMPL(half,       tc48_u32b, 24, tc48_u64b,  tc48_i64b,  tc48_pow3_u64,  tc48_to_u64,  tc48_from_u64)
TC48_ALU_IMPL(word,       tc48_u64b, 48, tc48_u128b, tc48_i128b, tc48_pow3_u128, tc48_to_u128, tc48_from_u128)
