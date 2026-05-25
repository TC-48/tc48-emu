#pragma once

#include <tc48/cpu/reg.h>
#include <tc48/cpu/imm.h>

#include <tc48/trit.h>
#include <tc48/word.h>

#include <stdio.h>

#define TC48_CPU_GPR_COUNT 32
#define TC48_CPU_REGS_COUNT \
    1 /* program counter */ \
  + 1 /* current state */   \
  + 1 /* always zero */     \
  + TC48_CPU_GPR_COUNT

#define TC48_CPU_REG_PC  0
#define TC48_CPU_REG_CPS 1 
#define TC48_CPU_REG_AZ  2

typedef struct tc48_cpu_regs {
    tc48_word data[TC48_CPU_REGS_COUNT];
} tc48_cpu_regs;

void tc48_cpu_set_reg_imm(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, const tc48_imm* imm);
void tc48_cpu_zero_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst);
void tc48_cpu_mov_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, tc48_reg_id src);

void tc48_cpu_dump_regs(tc48_cpu_regs* regs, FILE* out);

tc48_tryte   tc48_cpu_read_reg6 (tc48_cpu_regs* regs, tc48_reg_id r);
tc48_quarter tc48_cpu_read_reg12(tc48_cpu_regs* regs, tc48_reg_id r);
tc48_half    tc48_cpu_read_reg24(tc48_cpu_regs* regs, tc48_reg_id r);
tc48_word    tc48_cpu_read_reg48(tc48_cpu_regs* regs, tc48_reg_id r);

#define TC48_CPU_REG_MATH_DECL_TYPE(type)                                                                      \
    void tc48_cpu_min_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2);  \
    void tc48_cpu_max_##type##_reg (tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2); \
    void tc48_cpu_rot_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2);  \
    void tc48_cpu_not_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src);                     \
    void tc48_cpu_shl_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src, int count);          \
    void tc48_cpu_shr_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src, int count);          \
    void tc48_cpu_add_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2);  \
    void tc48_cpu_sub_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2);  \

TC48_CPU_REG_MATH_DECL_TYPE(doublet)
TC48_CPU_REG_MATH_DECL_TYPE(triplet)
TC48_CPU_REG_MATH_DECL_TYPE(quadruplet)
TC48_CPU_REG_MATH_DECL_TYPE(tryte)
TC48_CPU_REG_MATH_DECL_TYPE(quarter)
TC48_CPU_REG_MATH_DECL_TYPE(half)
TC48_CPU_REG_MATH_DECL_TYPE(word)

#undef TC48_CPU_REG_MATH_DECL_TYPE
