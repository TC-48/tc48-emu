#pragma once

#include <tc48/trit.h>
#include <tc48/word.h>

typedef union tc48_imm {
    tc48_tryte i6;
    tc48_quarter i12;
    tc48_half i24;
    tc48_word i48;
} tc48_imm;
