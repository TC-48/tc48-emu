#pragma once

#include <tc48/cpu/instr.h>
#include <tc48/cpu/regs.h>

#include <tc48/defs.h>

#include <stdint.h>
#include <stdbool.h>

struct tc48_system;
typedef struct tc48_system tc48_system;

typedef struct tc48_cpu {
    tc48_system* sys;

    tc48_cpu_regs regs;
    bool halted;
} tc48_cpu;

void tc48_cpu_init(tc48_cpu* cpu);
void tc48_cpu_deinit(tc48_cpu* cpu);

void tc48_cpu_reset(tc48_cpu* cpu);
void tc48_cpu_step(tc48_cpu* cpu);
void tc48_cpu_run(tc48_cpu* cpu, tc48_u64b cycles);
