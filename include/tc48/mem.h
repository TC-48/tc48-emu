#pragma once

#include <tc48/word.h>

#include <stddef.h>
#include <stdint.h>

typedef struct tc48_memory {
    tc48_word size;
    tc48_tryte data[];
} tc48_memory;

tc48_memory* tc48_mem_alloc(size_t size);
void tc48_mem_free(tc48_memory* mem);

tc48_tryte   tc48_mem_load6 (tc48_memory* mem, tc48_addr addr);
tc48_quarter tc48_mem_load12(tc48_memory* mem, tc48_addr addr);
tc48_half    tc48_mem_load24(tc48_memory* mem, tc48_addr addr);
tc48_word    tc48_mem_load48(tc48_memory* mem, tc48_addr addr);

void tc48_mem_store6(tc48_memory* mem, tc48_addr addr, tc48_tryte value);
void tc48_mem_store12(tc48_memory* mem, tc48_addr addr, tc48_quarter value);
void tc48_mem_store24(tc48_memory* mem, tc48_addr addr, tc48_half value);
void tc48_mem_store48(tc48_memory* mem, tc48_addr addr, tc48_word value);

