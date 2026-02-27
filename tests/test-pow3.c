#include <criterion/criterion.h>

#include <tc48/defs.h>
#include <tc48/pow3.h>

Test(pow3, u32) {
    tc48_u32b n = 1;
    for (tc48_usize i = 0; i < TC48_MAX_SAFE_TRITS_U32; ++i) {
        cr_assert_eq(tc48_pow3_u32[i], n);
        n *= 3;
    }
}

Test(pow3, u64) {
    tc48_u64b n = 1;
    for (tc48_usize i = 0; i < TC48_MAX_SAFE_TRITS_U64; ++i) {
        cr_assert_eq(tc48_pow3_u64[i], n);
        n *= 3;
    }
}

Test(pow3, u128) {
    tc48_u128b n = 1;
    for (tc48_usize i = 0; i < TC48_MAX_SAFE_TRITS_U128; ++i) {
        cr_assert_eq(tc48_pow3_u128[i], n);
        n *= 3;
    }
}
