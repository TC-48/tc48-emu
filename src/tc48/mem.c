#include <tc48/mem.h>
#include <tc48/pow3.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

tc48_memory* tc48_mem_alloc(tc48_word size) {
    const size_t tail_size = (size_t)size * sizeof(tc48_tryte);
    tc48_memory* mem = malloc(sizeof(tc48_memory) + tail_size);
    if (mem != NULL) {
        mem->size = size;
        memset(mem->data, 0, size * sizeof(tc48_tryte));
    }
    return mem;
}

void tc48_mem_free(tc48_memory* mem) {
    free(mem);
}

// TODO: better error handling
void tc48_mem_load_file(tc48_memory* mem, const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return;

    char magic[4];
    if (fread(magic, 1, 4, f) != 4 || memcmp(magic, "T48B", 4) != 0) {
        fputs("error: invalid T48B magic", stderr);
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 4, SEEK_SET);

    size_t tryte_count = (size - 4) / sizeof(tc48_tryte);
    fread(mem->data, sizeof(tc48_tryte), tryte_count, f);
    fclose(f);
}

tc48_tryte tc48_mem_load6(const tc48_memory* mem, tc48_word addr) {
    if (addr >= mem->size) return 0;
    return mem->data[addr];
}

tc48_quarter tc48_mem_load12(const tc48_memory* mem, tc48_word addr) {
    if (addr + 1 >= mem->size) return 0;
    return (tc48_quarter)mem->data[addr] + (tc48_quarter)mem->data[addr + 1] * TC48_TRYTE_VALUES;
}

tc48_half tc48_mem_load24(const tc48_memory* mem, tc48_word addr) {
    if (addr + 3 >= mem->size) return 0;
    tc48_half res = 0;
    tc48_u64b p = 1;
    for (tc48_u8b i = 0; i < 4; i++) {
        res += (tc48_half)mem->data[addr + i] * p;
        p *= TC48_TRYTE_VALUES;
    }
    return res;
}

tc48_word tc48_mem_load48(const tc48_memory* mem, tc48_word addr) {
    if (addr + 7 >= mem->size) return 0;
    tc48_word res = 0;
    tc48_u128b p = 1;
    for (tc48_u8b i = 0; i < 8; i++) {
        res += (tc48_word)mem->data[addr + i] * p;
        p *= TC48_TRYTE_VALUES;
    }
    return res;
}

void tc48_mem_store6(tc48_memory* mem, tc48_word addr, tc48_tryte value) {
    if (addr < mem->size) mem->data[addr] = value;
}

void tc48_mem_store12(tc48_memory* mem, tc48_word addr, tc48_quarter value) {
    if (addr + 1 < mem->size) {
        mem->data[addr]     = (tc48_tryte)(value % TC48_TRYTE_VALUES);
        mem->data[addr + 1] = (tc48_tryte)(value / TC48_TRYTE_VALUES);
    }
}

void tc48_mem_store24(tc48_memory* mem, tc48_word addr, tc48_half value) {
    if (addr + 3 < mem->size) {
        for (tc48_u8b i = 0; i < 4; i++) {
            mem->data[addr + i] = (tc48_tryte)(value % TC48_TRYTE_VALUES);
            value /= TC48_TRYTE_VALUES;
        }
    }
}

void tc48_mem_store48(tc48_memory* mem, tc48_word addr, tc48_word value) {
    if (addr + 7 < mem->size) {
        for (tc48_u8b i = 0; i < 8; i++) {
            mem->data[addr + i] = (tc48_tryte)(value % TC48_TRYTE_VALUES);
            value /= TC48_TRYTE_VALUES;
        }
    }
}

void tc48_mem_dump(const tc48_memory* mem, tc48_word addr, tc48_word size) {
    for (tc48_word i = 0; i < size; i++) {
        tc48_tryte t = tc48_mem_load6(mem, addr + i);
        printf("%04llu: ", (unsigned long long)(addr + i));
        for (int j = 0; j < 6; j++) {
            printf("%d", (int)tc48_tryte_get_trit(t, j));
        }
        printf(" (%d)\n", (int)t);
    }
}
