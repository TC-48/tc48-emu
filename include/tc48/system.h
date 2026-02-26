#pragma once

#include <tc48/cpu.h>
#include <tc48/mem.h>

typedef struct tc48_system {
    tc48_cpu cpus[256];
    tc48_memory* mem;
} tc48_system;

void tc48_system_init(tc48_system* sys, size_t mem_size);
void tc48_system_deinit(tc48_system* sys);

void tc48_system_reset(tc48_system* sys);
void tc48_system_step(tc48_system* sys);
