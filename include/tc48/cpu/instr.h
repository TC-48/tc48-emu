#pragma once

#include <tc48/cpu/reg.h>
#include <tc48/cpu/imm.h>

#include <tc48/word.h>

// NOTE: represented as a doublet
enum tc48_instr_format {
    TC48_INSTR_FORMAT_NONE,
    TC48_INSTR_FORMAT_R,
    TC48_INSTR_FORMAT_RRA,
    TC48_INSTR_FORMAT_RR,
    TC48_INSTR_FORMAT_RRR,
    TC48_INSTR_FORMAT_RI,
    TC48_INSTR_FORMAT_RRI,
    TC48_INSTR_FORMAT_IR,
    TC48_INSTR_FORMAT_IRR,
};

// NOTE: represented as a doublet
enum tc48_operand_width {
    TC48_OPERAND_WIDTH_6,
    TC48_OPERAND_WIDTH_12,
    TC48_OPERAND_WIDTH_24,
    TC48_OPERAND_WIDTH_48,
};

// NOTE: represented as a triplet
enum tc48_pred {
    TC48_PRED_AW, /// if AlWays

    TC48_PRED_EQ, /// if EQual         (S == 0)
    TC48_PRED_NE, /// if Not-Equal     (S != 0)
    TC48_PRED_LT, /// if Less-Than     (S == 2)
    TC48_PRED_GT, /// if Greater-Than  (S == 1)
    TC48_PRED_LE, /// if Less-Equal    (S == 0 or S == 2)
    TC48_PRED_GE, /// if Greater-Equal (S == 0 or S == 1)
    TC48_PRED_ZR, /// if ZeRo          (S == 0)
    TC48_PRED_NZ, /// if Non-Zero      (S != 0)

    TC48_PRED_CS, /// if Carry-Set    (C == 1)
    TC48_PRED_CC, /// if Carry-Clear  (C != 1)
    TC48_PRED_BS, /// if Borrow-Set   (C == 2)
    TC48_PRED_BC, /// if Borrow-Clear (C != 2)

    TC48_PRED_VS, /// if oVerflow-set          (V != 0)
    TC48_PRED_VP, /// if oVerflow-set-Positive (V == 1)
    TC48_PRED_VN, /// if oVerflow-set-Negative (V == 2)
    TC48_PRED_VC, /// if oVerflow-set-Clear    (V == 0)
};

#define TC48_WCFR_NONE TC48_ZERO
#define TC48_WCFR_STAT TC48_ONE
#define TC48_WCFR_FULL TC48_TWO

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
            tc48_reg_id r2;
            tc48_word addr;
        } rra;
        struct {
            tc48_imm imm;
            tc48_reg_id r1;
            tc48_reg_id r2;
        } irr;
        struct {
            tc48_imm imm;
            tc48_reg_id r1;
        } ir;
     } operands;
} tc48_instr;

typedef enum tc48_operand_width tc48_width;
