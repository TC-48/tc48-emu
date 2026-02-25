#pragma once

#include <tc48/trit.h>
#include <tc48/word.h>

typedef struct tc48_reg_id {
    tc48_quadruplet base;
    tc48_doublet lane;
} tc48_reg_id;
