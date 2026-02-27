#include <criterion/criterion.h>

#include <tc48/mem.h>
#include <tc48/word.h>
#include <tc48/trit.h>

Test(mem, load_store_6) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_addr addr = TC48_WORD(
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
    );
    
    tc48_tryte val = TC48_TRYTE(0, 1, 2, 0, 1, 2);
    
    tc48_mem_store6(mem, addr, val);
    cr_assert(TC48_TRYTE_EQL(tc48_mem_load6(mem, addr), val));
    
    tc48_mem_free(mem);
}

Test(mem, load_store_12) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_addr addr = TC48_WORD(
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2
    );
    
    tc48_quarter val = TC48_QUARTER(0,1,2,0,1,2, 0,1,2,0,1,2);
    
    tc48_mem_store12(mem, addr, val);
    cr_assert(TC48_QUARTER_EQL(tc48_mem_load12(mem, addr), val));
    
    // verify tryte layout: low tryte
    tc48_tryte low = tc48_mem_load6(mem, addr);
    cr_assert(TC48_TRYTE_EQL(low, TC48_TRYTE(0,1,2,0,1,2)));
    
    // verify tryte layout: high tryte (addr = 3)
    tc48_addr addr_plus_1 = TC48_WORD(
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0
    );
    tc48_tryte high = tc48_mem_load6(mem, addr_plus_1);
    cr_assert(TC48_TRYTE_EQL(high, TC48_TRYTE(0,1,2,0,1,2)));
    
    tc48_mem_free(mem);
}

Test(mem, load_store_24) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_addr addr = TC48_WORD_ZERO;
    
    tc48_half val = TC48_HALF(
        0,1,2,0,1,2, 0,1,2,0,1,2,
        2,1,0,2,1,0, 2,1,0,2,1,0
    );
    
    tc48_mem_store24(mem, addr, val);
    cr_assert(TC48_HALF_EQL(tc48_mem_load24(mem, addr), val));
    
    tc48_mem_free(mem);
}

Test(mem, load_store_48) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_addr addr = TC48_WORD_ZERO;
    
    tc48_word val = TC48_WORD(
        0,1,2,0,1,2, 0,1,2,0,1,2, 0,1,2,0,1,2, 0,1,2,0,1,2,
        2,1,0,2,1,0, 2,1,0,2,1,0, 2,1,0,2,1,0, 2,1,0,2,1,0
    );
    
    tc48_mem_store48(mem, addr, val);
    cr_assert(TC48_WORD_EQL(tc48_mem_load48(mem, addr), val));
    
    tc48_mem_free(mem);
}
