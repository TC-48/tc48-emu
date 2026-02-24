#include <tc48/mem.h>

#include <stdlib.h>
#include <string.h>

tc48_memory* tc48_mem_alloc(size_t size) {
    tc48_memory* mem = malloc(sizeof(tc48_memory) + size * sizeof(tc48_tryte));
    if (mem) {
        mem->size = (tc48_word)size;
        memset(mem->data, 0, size * sizeof(tc48_tryte));
    }
    return mem;
}

void tc48_mem_free(tc48_memory* mem) {
    free(mem);
}

tc48_tryte tc48_mem_load6(tc48_memory* mem, tc48_addr addr) {
    if (addr >= mem->size) {
        return 0;
    }
    return mem->data[addr];
}

tc48_quarter tc48_mem_load12(tc48_memory* mem, tc48_addr addr) {
    tc48_quarter val = 0;
    val += (tc48_quarter) tc48_mem_load6(mem, addr);
    val += (tc48_quarter) tc48_mem_load6(mem, addr + 1) * TC48_TRYTE_VALUES;
    return val;
}

tc48_half tc48_mem_load24(tc48_memory* mem, tc48_addr addr) {
    tc48_half val = 0;
    val += (tc48_half) tc48_mem_load12(mem, addr);
    val += (tc48_half) tc48_mem_load12(mem, addr + 2) * TC48_QUARTER_VALUES;
    return val;
}

tc48_word tc48_mem_load48(tc48_memory* mem, tc48_addr addr) {
    tc48_word val = 0;
    val += (tc48_word) tc48_mem_load24(mem, addr);
    val += (tc48_word) tc48_mem_load24(mem, addr + 4) * TC48_HALF_VALUES;
    return val;
}

void tc48_mem_store6(tc48_memory* mem, tc48_addr addr, tc48_tryte value) {
    if (addr < mem->size) {
        mem->data[addr] = value;
    }
}

void tc48_mem_store12(tc48_memory* mem, tc48_addr addr, tc48_quarter value) {
    tc48_mem_store6(mem, addr, (tc48_tryte) (value % TC48_TRYTE_VALUES));
    tc48_mem_store6(mem, addr + 1, (tc48_tryte) (value / TC48_TRYTE_VALUES));
}

void tc48_mem_store24(tc48_memory* mem, tc48_addr addr, tc48_half value) {
    tc48_mem_store12(mem, addr, (tc48_quarter) (value % TC48_QUARTER_VALUES));
    tc48_mem_store12(mem, addr + 2, (tc48_quarter) (value / TC48_QUARTER_VALUES));
}

void tc48_mem_store48(tc48_memory* mem, tc48_addr addr, tc48_word value) {
    tc48_mem_store24(mem, addr, (tc48_half) (value % TC48_HALF_VALUES));
    tc48_mem_store24(mem, addr + 4, (tc48_half) (value / TC48_HALF_VALUES));
}
