#pragma once

#include <tc48/cpu/reg.h>

#include <tc48/trit.h>
#include <tc48/word.h>

#define TC48_CPU_GPR_COUNT 32
#define TC48_CPU_REGS_COUNT \
    1 /* program counter */ + TC48_CPU_GPR_COUNT

typedef struct tc48_cpu_regs {
    tc48_word data[TC48_CPU_REGS_COUNT];
} tc48_cpu_regs;

void tc48_cpu_set_reg_imm6 (tc48_cpu_regs* regs, tc48_reg_id dst, tc48_tryte imm);
void tc48_cpu_set_reg_imm12(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_quarter imm);
void tc48_cpu_set_reg_imm24(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_half imm);
void tc48_cpu_set_reg_imm48(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_word imm);

void tc48_cpu_set_reg6 (tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src);
void tc48_cpu_set_reg12(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src);
void tc48_cpu_set_reg24(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src);
void tc48_cpu_set_reg48(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src);
