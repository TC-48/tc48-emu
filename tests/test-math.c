#include <criterion/criterion.h>
#include <tc48/cpu/math.h>
#include <tc48/word.h>

Test(math, tryte_tritwise) {
    tc48_tryte a = TC48_TRYTE(0, 1, 2, 0, 1, 2);
    tc48_tryte b = TC48_TRYTE(2, 1, 0, 2, 1, 0);
    
    // AND (min)
    // 0&2=0, 1&1=1, 2&0=0, 0&2=0, 1&1=1, 2&0=0
    tc48_tryte res_and = tc48_math_tryte_and(a, b);
    cr_assert(TC48_TRYTE_EQL(res_and, TC48_TRYTE(0, 1, 0, 0, 1, 0)));
    
    // OR (max)
    // 0|2=2, 1|1=1, 2|0=2, 0|2=2, 1|1=1, 2|0=2
    tc48_tryte res_or = tc48_math_tryte_or(a, b);
    cr_assert(TC48_TRYTE_EQL(res_or, TC48_TRYTE(2, 1, 2, 2, 1, 2)));
    
    // XOR (sum mod 3)
    // 0+2=2, 1+1=2, 2+0=2, 0+2=2, 1+1=2, 2+0=2
    tc48_tryte res_xor = tc48_math_tryte_xor(a, b);
    cr_assert(TC48_TRYTE_EQL(res_xor, TC48_TRYTE(2, 2, 2, 2, 2, 2)));

    // NOT (2 - trit)
    // 2-0=2, 2-1=1, 2-2=0, 2-0=2, 2-1=1, 2-2=0
    tc48_tryte res_not = tc48_math_tryte_not(a);
    cr_assert(TC48_TRYTE_EQL(res_not, TC48_TRYTE(2, 1, 0, 2, 1, 0)));
}

Test(math, tryte_shift) {
    tc48_tryte a = TC48_TRYTE(0, 1, 2, 0, 1, 2);
    
    // SHL 1
    tc48_tryte res_shl = tc48_math_tryte_shl(a, 1);
    cr_assert(TC48_TRYTE_EQL(res_shl, TC48_TRYTE(1, 2, 0, 1, 2, 0)));
    
    // SHR 1
    tc48_tryte res_shr = tc48_math_tryte_shr(a, 1);
    cr_assert(TC48_TRYTE_EQL(res_shr, TC48_TRYTE(0, 0, 1, 2, 0, 1)));
}
