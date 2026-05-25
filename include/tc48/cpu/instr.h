#pragma once

#include <tc48/cpu/reg.h>
#include <tc48/cpu/imm.h>

#include <tc48/word.h>

enum tc48_instr_format: tc48_triplet {
    TC48_INSTR_FORMAT_NONE,
    TC48_INSTR_FORMAT_R,
    TC48_INSTR_FORMAT_A,
    TC48_INSTR_FORMAT_I,
    TC48_INSTR_FORMAT_RR,
    TC48_INSTR_FORMAT_RRR,
    TC48_INSTR_FORMAT_RI,
    TC48_INSTR_FORMAT_RRI,
    TC48_INSTR_FORMAT_RA,
    TC48_INSTR_FORMAT_RRA,
};

enum tc48_operand_width: tc48_doublet {
    TC48_OPERAND_WIDTH_6,
    TC48_OPERAND_WIDTH_12,
    TC48_OPERAND_WIDTH_24,
    TC48_OPERAND_WIDTH_48,
};

typedef struct tc48_instr {
    tc48_triplet format;
    tc48_doublet width;
    // 1 trit padding
    tc48_tryte opcode;
    union {
        struct {
            tc48_reg_id r1;
        } r;
        struct {
            tc48_addr addr;
        } a;
        struct {
            tc48_imm imm;
        } i;
        struct {
            tc48_reg_id r1;
            tc48_reg_id r2;
        } rr;
        struct {
            tc48_reg_id r1;
            tc48_reg_id r2;
            tc48_reg_id r3;
        } rrr;
        struct {
            tc48_reg_id r1;
            tc48_imm imm;
        } ri;
        struct {
            tc48_reg_id r1;
            tc48_reg_id r2;
            tc48_imm imm;
        } rri;
        struct {
            tc48_reg_id r1;
            tc48_addr addr;
        } ra;
        struct {
            tc48_reg_id r1;
            tc48_reg_id r2;
            tc48_addr addr;
        } rra;
    } operands;
} tc48_instr;
