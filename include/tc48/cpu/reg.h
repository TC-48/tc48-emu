#pragma once

#include <tc48/trit.h>
#include <tc48/word.h>

typedef struct tc48_reg_id {
    tc48_quadruplet base;
    tc48_doublet lane;
} tc48_reg_id;

#define TC48_WHOLE_REG(BASE) ((tc48_reg_id) { .base = BASE, .lane = 0 })

#define TC48_CF_TRIT_S 0
#define TC48_CF_TRIT_V 1
#define TC48_CF_TRIT_C 2

#define TC48_CF_S_ZERO TC48_ZERO
#define TC48_CF_S_POS  TC48_ONE
#define TC48_CF_S_NEG  TC48_TWO

#define TC48_CF_C_NONE   TC48_ZERO
#define TC48_CF_C_CARRY  TC48_ONE
#define TC48_CF_C_BORROW TC48_TWO

#define TC48_CF_V_NONE TC48_ZERO
#define TC48_CF_V_POS TC48_ONE
#define TC48_CF_V_NEG TC48_TWO
