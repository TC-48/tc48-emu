#pragma once

#include <tc48/cpu/reg.h>
#include <tc48/cpu/imm.h>

#include <tc48/word.h>

enum tc48_instr_format: tc48_doublet {
    TC48_INSTR_FORMAT_NONE,
    TC48_INSTR_FORMAT_R,
    TC48_INSTR_FORMAT_I,
    TC48_INSTR_FORMAT_RR,
    TC48_INSTR_FORMAT_RRR,
    TC48_INSTR_FORMAT_RI,
    TC48_INSTR_FORMAT_RRI,
    TC48_INSTR_FORMAT_RIR,
};

enum tc48_operand_width: tc48_doublet {
    TC48_OPERAND_WIDTH_6,
    TC48_OPERAND_WIDTH_12,
    TC48_OPERAND_WIDTH_24,
    TC48_OPERAND_WIDTH_48,
};

enum tc48_pred: tc48_triplet {
    TC48_PRED_AW, /// if AlWays
    TC48_PRED_EQ, /// if EQual
    TC48_PRED_NE, /// if Not-Equal
    TC48_PRED_LT, /// if Less-Than
    TC48_PRED_GT, /// if Greater-Than
    TC48_PRED_ZR, /// if ZeRo
    TC48_PRED_NZ, /// if Non-Zero
    TC48_PRED_CS, /// if Carry-Set
    TC48_PRED_CC, /// if Carry-Clear
    TC48_PRED_VS, /// if oVerflow-Set
    TC48_PRED_VC, /// if oVerflow-Clear
};

typedef struct tc48_instr {
    tc48_doublet    format;
    tc48_doublet    width;
    tc48_trit_state wcfr;
    tc48_triplet    pred;
    tc48_quadruplet opcode;
    union {
        struct {
            tc48_reg_id r1;
        } r;
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
            tc48_imm imm;
            tc48_reg_id r2;
        } rir;
    } operands;
} tc48_instr;
