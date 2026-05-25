#include <criterion/criterion.h>
#include <tc48/cpu/math.h>
#include <tc48/word.h>

Test(math,tryte_tritwise) {
    tc48_tryte a = TC48_TRYTE(0,1,2,0,1,2);
    tc48_tryte b = TC48_TRYTE(2,1,0,2,1,0);
    
    // MIN
    // 0&2=0,1&1=1,2&0=0,0&2=0,1&1=1,2&0=0
    tc48_tryte res_min = tc48_math_tryte_min(a, b);
    cr_assert(res_min == TC48_TRYTE(0,1,0,0,1,0));
    
    // MAX
    // 0|2=2,1|1=1,2|0=2,0|2=2,1|1=1,2|0=2
    tc48_tryte res_max = tc48_math_tryte_max(a, b);
    cr_assert(res_max == TC48_TRYTE(2,1,2,2,1,2));
    
    // ROT (sum mod 3)
    // 0+2=2,1+1=2,2+0=2,0+2=2,1+1=2,2+0=2
    tc48_tryte res_xmax = tc48_math_tryte_rot(a, b);
    cr_assert(res_xmax == TC48_TRYTE(2,2,2,2,2,2));

    // NOT (2 - trit)
    // 2-0=2,2-1=1,2-2=0,2-0=2,2-1=1,2-2=0
    tc48_tryte res_not = tc48_math_tryte_not(a);
    cr_assert(res_not == TC48_TRYTE(2,1,0,2,1,0));
}

Test(math,tryte_shift) {
    tc48_tryte a = TC48_TRYTE(0,1,2,0,1,2);
    
    // SHL 1
    tc48_tryte res_shl = tc48_math_tryte_shl(a, 1);
    cr_assert(res_shl == TC48_TRYTE(1,2,0,1,2,0));
    
    // SHR 1
    tc48_tryte res_shr = tc48_math_tryte_shr(a, 1);
    cr_assert(res_shr == TC48_TRYTE(0,0,1,2,0,1));
}
