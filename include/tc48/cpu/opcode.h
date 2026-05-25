#pragma once

#include <tc48/word.h>

enum tc48_opcode: tc48_tryte {
    TC48_OP_MIN, /// min; format RRR or RRI
    TC48_OP_MAX, /// max; format RRR or RRI
    TC48_OP_ROT, /// rot; format RRR or RRI
    TC48_OP_NOT, /// not; format RRR or RRI
    TC48_OP_SHL, /// shl; format RRR or RRI
    TC48_OP_SHR, /// shl; format RRR or RRI

    TC48_OP_ADD, /// add; format: RRR or RRI
    TC48_OP_SUB, /// sub; format: RRR or RRI
    TC48_OP_MUL, /// mul; format: RRR or RRI
    TC48_OP_DIV, /// div; format: RRR or RRI

    TC48_OP_SMUL, /// signed mul; format: RRR or RRI
    TC48_OP_SDIV, /// signed div; format: RRR or RRI

    TC48_OP_IN,  /// input;  format: RR or RI
    TC48_OP_OUT, /// output; format: RR or RI
    
    TC48_OP_LOAD,   /// load;         format: RI, RR
    TC48_OP_STORE,  /// store;        format: RI, RR
    TC48_OP_OLOAD,  /// offset load;  format: RRI or RIR
    TC48_OP_OSTORE, /// offset store; format: RRI or RIR

    TC48_OP_JMP,      /// jump; format: R or I
    TC48_OP_JMPIF_EQ, /// jump if EQual;            format: R or I
    TC48_OP_JMPIF_NE, /// jump if Not-Equal;        format: R or I
    TC48_OP_JMPIF_LT, /// jump if Less-Than;        format: R or I
    TC48_OP_JMPIF_GT, /// jump if Greater-Than;     format: R or I
    TC48_OP_JMPIF_ZR, /// jump if ZeRo;             format: R or I
    TC48_OP_JMPIF_NZ, /// jump if Non-Zero;         format: R or I
    TC48_OP_JMPIF_CS, /// jump if Carry-Set;        format: R or I
    TC48_OP_JMPIF_CC, /// jump if Carry-Clear;      format: R or I
    TC48_OP_JMPIF_VS, /// jump if oVerflow-Set;     format: R or I
    TC48_OP_JMPIF_VC, /// jump if oVerflow-Clear;   format: R or I

    TC48_OP_NOP,  /// no-op; format: NONE
    TC48_OP_HALT, /// halt;  format: NONE
};
