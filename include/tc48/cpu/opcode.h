#pragma once

#include <tc48/word.h>

enum tc48_opcode: tc48_quadruplet {
    TC48_OP_NOP,  /// no-op; format: NONE
    TC48_OP_HALT, /// halt;  format: NONE

    TC48_OP_MIN, /// min; format RRR or RRI
    TC48_OP_MAX, /// max; format RRR or RRI
    TC48_OP_ROT, /// rot; format RRR or RRI
    TC48_OP_SHL, /// shl; format RRR or RRI
    TC48_OP_SHR, /// shl; format RRR or RRI

    TC48_OP_NOT, /// not; format RR or RI

    TC48_OP_ADD, /// add; format: RRR or RRI
    TC48_OP_SUB, /// sub; format: RRR or RRI
    TC48_OP_NEG, /// neg; format: RR or RI
    
    TC48_OP_UMUL, /// unsigned mul; format: RRR or RRI
    TC48_OP_UDIV, /// unsigned div; format: RRR or RRI
    TC48_OP_SMUL, /// signed mul;   format: RRR or RRI
    TC48_OP_SDIV, /// signed div;   format: RRR or RRI

    TC48_OP_IN,  /// input;  format: RRR or RRI
    TC48_OP_OUT, /// output; format: RRR or RRI

    TC48_OP_LOAD,  /// load;  format: RRR, RRI
    TC48_OP_STORE, /// store; format: RRR, RRI
};
