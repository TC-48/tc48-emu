#include <tc48/mem.h>
#include <tc48/pow3.h>

#include <stdlib.h>
#include <string.h>

static inline size_t addr_to_idx(tc48_addr a) {
    size_t i = 0;
    for (int j = 0; j < TC48_MAX_SAFE_TRITS; j++) {
        tc48_u8b t = ((a.l >> j) & 1) | (((a.h >> j) & 1) << 1);
        if (t) i += t * tc48_pow3[j];
    }
    return i;
}

static inline tc48_word idx_to_addr(size_t i) {
    tc48_word a = { 0, 0 };
    for (int j = 0; i > 0 && j < TC48_MAX_SAFE_TRITS; j++, i /= 3) {
        size_t r = i % 3;
        a.l |= (tc48_u64b) (r & 1) << j;
        a.h |= (tc48_u64b) (r >> 1) << j;
    }
    return a;
}

static inline int addr_ge(tc48_addr a, tc48_addr b) {
    if (a.h != b.h) return a.h > b.h;
    return a.l >= b.l;
}

tc48_memory* tc48_mem_alloc(size_t size) {
    tc48_memory* mem = malloc(sizeof(tc48_memory) + size * sizeof(tc48_tryte));
    if (mem) {
        mem->size = idx_to_addr(size);
        memset(mem->data, 0, size * sizeof(tc48_tryte));
    }
    return mem;
}

void tc48_mem_free(tc48_memory* mem) {
    free(mem);
}

tc48_tryte tc48_mem_load6(tc48_memory* mem, tc48_addr addr) {
    if (addr_ge(addr, mem->size)) return (tc48_tryte){0, 0};
    return mem->data[addr_to_idx(addr)];
}

tc48_quarter tc48_mem_load12(tc48_memory* mem, tc48_addr addr) {
    size_t idx = addr_to_idx(addr);
    tc48_quarter q = {0, 0};
    q.l = mem->data[idx].l | ((tc48_u16b)mem->data[idx + 1].l << 6);
    q.h = mem->data[idx].h | ((tc48_u16b)mem->data[idx + 1].h << 6);
    return q;
}

tc48_half tc48_mem_load24(tc48_memory* mem, tc48_addr addr) {
    size_t idx = addr_to_idx(addr);
    tc48_half h = {0, 0};
    for (int i = 0; i < 4; i++) {
        h.l |= (tc48_u32b)mem->data[idx + i].l << (i * 6);
        h.h |= (tc48_u32b)mem->data[idx + i].h << (i * 6);
    }
    return h;
}

tc48_word tc48_mem_load48(tc48_memory* mem, tc48_addr addr) {
    size_t idx = addr_to_idx(addr);
    tc48_word w = {0, 0};
    for (int i = 0; i < 8; i++) {
        w.l |= (tc48_u64b) mem->data[idx + i].l << (i * 6);
        w.h |= (tc48_u64b) mem->data[idx + i].h << (i * 6);
    }
    return w;
}

void tc48_mem_store6(tc48_memory* mem, tc48_addr addr, tc48_tryte value) {
    mem->data[addr_to_idx(addr)] = value;
}

void tc48_mem_store12(tc48_memory* mem, tc48_addr addr, tc48_quarter value) {
    size_t idx = addr_to_idx(addr);
    mem->data[idx].l = value.l & 0x3F;
    mem->data[idx].h = value.h & 0x3F;
    mem->data[idx + 1].l = (value.l >> 6) & 0x3F;
    mem->data[idx + 1].h = (value.h >> 6) & 0x3F;
}

void tc48_mem_store24(tc48_memory* mem, tc48_addr addr, tc48_half value) {
    size_t idx = addr_to_idx(addr);
    for (int i = 0; i < 4; i++) {
        mem->data[idx + i].l = (value.l >> (i * 6)) & 0x3F;
        mem->data[idx + i].h = (value.h >> (i * 6)) & 0x3F;
    }
}

void tc48_mem_store48(tc48_memory* mem, tc48_addr addr, tc48_word value) {
    size_t idx = addr_to_idx(addr);
    for (int i = 0; i < 8; i++) {
        mem->data[idx + i].l = (value.l >> (i * 6)) & 0x3F;
        mem->data[idx + i].h = (value.h >> (i * 6)) & 0x3F;
    }
}
