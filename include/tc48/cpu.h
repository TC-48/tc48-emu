#pragma once

#include <stdint.h>

struct tc48_system;
typedef struct tc48_system tc48_system;

typedef struct tc48_cpu {
    tc48_system* sys;
} tc48_cpu;

void tc48_cpu_init(tc48_cpu* cpu);
void tc48_cpu_deinit(tc48_cpu* cpu);

void tc48_cpu_reset(tc48_cpu* cpu);
void tc48_cpu_step(tc48_cpu* cpu);
void tc48_cpu_run(tc48_cpu* cpu, uint64_t cycles);
