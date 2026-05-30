#pragma once

#include <tc48/cpu/reg.h>
#include <tc48/cpu/imm.h>

#include <tc48/trit.h>
#include <tc48/word.h>

#include <stdio.h>

#define TC48_CPU_GPR_COUNT 32
#define TC48_CPU_REGS_COUNT \
    1 /* always zero */     \
  + 1 /* current flags */   \
  + 1 /* instr pointer */   \
  + 1 /* stack pointer */   \
  + TC48_CPU_GPR_COUNT

#define TC48_CPU_REG_AZ 0
#define TC48_CPU_REG_CF 1 
#define TC48_CPU_REG_IP 2
#define TC48_CPU_REG_SP 3

#define TC48_CPU_GPR_BASE 4

typedef struct tc48_cpu_regs {
    tc48_word data[TC48_CPU_REGS_COUNT];
} tc48_cpu_regs;

void tc48_cpu_dump_regs(tc48_cpu_regs* regs, FILE* out);

tc48_tryte   tc48_cpu_read_reg6 (tc48_cpu_regs* regs, tc48_reg_id r);
tc48_quarter tc48_cpu_read_reg12(tc48_cpu_regs* regs, tc48_reg_id r);
tc48_half    tc48_cpu_read_reg24(tc48_cpu_regs* regs, tc48_reg_id r);
tc48_word    tc48_cpu_read_reg48(tc48_cpu_regs* regs, tc48_reg_id r);

void tc48_cpu_write_reg6 (tc48_cpu_regs* regs, tc48_reg_id r, tc48_tryte val);
void tc48_cpu_write_reg12(tc48_cpu_regs* regs, tc48_reg_id r, tc48_quarter val);
void tc48_cpu_write_reg24(tc48_cpu_regs* regs, tc48_reg_id r, tc48_half val);
void tc48_cpu_write_reg48(tc48_cpu_regs* regs, tc48_reg_id r, tc48_word val);

#define TC48_CPU_REG_MATH_DECL_TYPE(type)                                                                                            \
    void tc48_cpu_min_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr);  \
    void tc48_cpu_max_##type##_reg (tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr); \
    void tc48_cpu_rot_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr);  \
    void tc48_cpu_not_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src, tc48_trit_state wcfr);                     \
    void tc48_cpu_neg_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src, tc48_trit_state wcfr);                     \
    void tc48_cpu_shl_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr);  \
    void tc48_cpu_shr_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr);  \
    void tc48_cpu_add_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr);  \
    void tc48_cpu_sub_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr);  \
    void tc48_cpu_umul_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr); \
    void tc48_cpu_udiv_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr); \
    void tc48_cpu_smul_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr); \
    void tc48_cpu_sdiv_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr); \
    void tc48_cpu_min_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_max_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_rot_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_not_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_##type imm, tc48_trit_state wcfr);                     \
    void tc48_cpu_neg_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_##type imm, tc48_trit_state wcfr);                     \
    void tc48_cpu_shl_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_shr_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_add_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_sub_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);   \
    void tc48_cpu_umul_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);  \
    void tc48_cpu_udiv_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);  \
    void tc48_cpu_smul_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);  \
    void tc48_cpu_sdiv_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr);  \

TC48_CPU_REG_MATH_DECL_TYPE(doublet)
TC48_CPU_REG_MATH_DECL_TYPE(triplet)
TC48_CPU_REG_MATH_DECL_TYPE(quadruplet)
TC48_CPU_REG_MATH_DECL_TYPE(tryte)
TC48_CPU_REG_MATH_DECL_TYPE(quarter)
TC48_CPU_REG_MATH_DECL_TYPE(half)
TC48_CPU_REG_MATH_DECL_TYPE(word)

#undef TC48_CPU_REG_MATH_DECL_TYPE
