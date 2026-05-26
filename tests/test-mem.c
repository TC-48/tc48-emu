#include <criterion/criterion.h>

#include <tc48/mem.h>
#include <tc48/word.h>
#include <tc48/trit.h>

Test(mem, load_store_6) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_word addr = 1;

    tc48_tryte val = TC48_TRYTE(0, 1, 2, 0, 1, 2);

    tc48_mem_store6(mem, addr, val);
    cr_assert_eq(tc48_mem_load6(mem, addr), val);

    tc48_mem_free(mem);
}

Test(mem, load_store_12) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_word addr = 2;

    tc48_quarter val = TC48_QUARTER(0,1,2,0,1,2, 0,1,2,0,1,2);

    tc48_mem_store12(mem, addr, val);
    cr_assert_eq(tc48_mem_load12(mem, addr), val);

    tc48_tryte low = tc48_mem_load6(mem, addr);
    cr_assert_eq(low, TC48_TRYTE(0,1,2,0,1,2));

    tc48_word addr_plus_1 = 3;
    tc48_tryte high = tc48_mem_load6(mem, addr_plus_1);
    cr_assert_eq(high, TC48_TRYTE(0,1,2,0,1,2));

    tc48_mem_free(mem);
}

Test(mem, load_store_24) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_word addr = 0;

    tc48_half val = TC48_HALF(
        0,1,2,0,1,2, 0,1,2,0,1,2,
        2,1,0,2,1,0, 2,1,0,2,1,0
    );

    tc48_mem_store24(mem, addr, val);
    cr_assert_eq(tc48_mem_load24(mem, addr), val);

    tc48_mem_free(mem);
}

Test(mem, load_store_48) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_word addr = 0;

    tc48_word val = TC48_WORD(
        0,1,2,0,1,2, 0,1,2,0,1,2, 0,1,2,0,1,2, 0,1,2,0,1,2,
        2,1,0,2,1,0, 2,1,0,2,1,0, 2,1,0,2,1,0, 2,1,0,2,1,0
    );

    tc48_mem_store48(mem, addr, val);
    cr_assert_eq(tc48_mem_load48(mem, addr), val);

    tc48_mem_free(mem);
}
