#pragma once

#include <tc48/word.h>

#define TC48_INSTR_FORMAT_R   TC48_DOUBLET(0, 0)
#define TC48_INSTR_FORMAT_RR  TC48_DOUBLET(0, 1)
#define TC48_INSTR_FORMAT_RRR TC48_DOUBLET(0, 2)
#define TC48_INSTR_FORMAT_RI  TC48_DOUBLET(1, 0)
#define TC48_INSTR_FORMAT_RA  TC48_DOUBLET(1, 1)
#define TC48_INSTR_FORMAT_RRA TC48_DOUBLET(1, 2)

#define TC48_OPERAND_WIDTH_6  TC48_DOUBLET(0, 0)
#define TC48_OPERAND_WIDTH_12 TC48_DOUBLET(0, 1)
#define TC48_OPERAND_WIDTH_24 TC48_DOUBLET(0, 2)
#define TC48_OPERAND_WIDTH_48 TC48_DOUBLET(1, 0)

typedef struct tc48_regid {
    tc48_quadruplet base;
    tc48_doublet lane;
} tc48_regid;

typedef union tc48_imm {
    tc48_tryte i6;
    tc48_quarter i12;
    tc48_half i24;
    tc48_word i48;
} tc48_imm;

typedef struct tc48_instr {
    tc48_doublet format;
    tc48_tryte opcode;
    tc48_doublet width;
    union {
        struct {
            tc48_regid r1;
        } r;
        struct {
            tc48_regid r1;
            tc48_regid r2;
        } rr;
        struct {
            tc48_regid r1;
            tc48_regid r2;
            tc48_regid r3;
        } rrr;
        struct {
            tc48_regid r1;
            tc48_imm imm;
        } ri;
        struct {
            tc48_regid r1;
            tc48_addr addr;
        } ra;
        struct {
            tc48_regid r1;
            tc48_regid r2;
            tc48_addr addr;
        } rra;
    } operands;
} tc48_instr;
