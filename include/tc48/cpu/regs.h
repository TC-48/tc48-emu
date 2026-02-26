#pragma once

#include <tc48/cpu/reg.h>
#include <tc48/cpu/imm.h>

#include <tc48/trit.h>
#include <tc48/word.h>

#define TC48_CPU_GPR_COUNT 32
#define TC48_CPU_REGS_COUNT \
    1 /* program counter */ + TC48_CPU_GPR_COUNT

typedef struct tc48_cpu_regs {
    tc48_word data[TC48_CPU_REGS_COUNT];
} tc48_cpu_regs;

void tc48_cpu_set_reg_imm(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, tc48_imm* imm);
void tc48_cpu_set_zero_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst);
void tc48_cpu_mov_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, tc48_reg_id src);
