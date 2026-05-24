#include <tc48/cpu/regs.h>

#include <tc48/cpu/instr.h>
#include <tc48/cpu/math.h>

static inline tc48_usize reg_to_idx(tc48_reg_id r) {
    return (tc48_usize)r.base;
}
static inline tc48_usize lane_to_off(tc48_reg_id r) {
    return (tc48_usize)r.lane;
}

#define GEN_ACCESSORS(name, type, bits, values)                                                     \
    static type get_##name(tc48_cpu_regs* regs, tc48_reg_id r) {                                    \
        tc48_usize idx = reg_to_idx(r);                                                             \
        tc48_usize off = lane_to_off(r) * bits;                                                     \
        tc48_word w = regs->data[idx];                                                              \
        return (type)((w / tc48_pow3_u128[off]) % (values));                                        \
    }                                                                                               \
    static void set_##name(tc48_cpu_regs* regs, tc48_reg_id r, type val) {                          \
        tc48_usize idx = reg_to_idx(r);                                                             \
        tc48_usize off = lane_to_off(r) * bits;                                                     \
        tc48_u128b p = tc48_pow3_u128[off];                                                         \
        tc48_word w = regs->data[idx];                                                              \
        tc48_word current_val = (w / p) % (values);                                                 \
        regs->data[idx] = w - (current_val * p) + ((tc48_word)val * p);                             \
    }

GEN_ACCESSORS(tryte,      tc48_tryte,      6,  TC48_TRYTE_VALUES)
GEN_ACCESSORS(quarter,    tc48_quarter,    12, TC48_QUARTER_VALUES)
GEN_ACCESSORS(half,       tc48_half,       24, TC48_HALF_VALUES)
GEN_ACCESSORS(word,       tc48_word,       48, TC48_WORD_VALUES)

void tc48_cpu_set_reg_imm(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, const tc48_imm* imm) {
    if      (width == TC48_OPERAND_WIDTH_6)  set_tryte(regs, dst, imm->i6);
    else if (width == TC48_OPERAND_WIDTH_12) set_quarter(regs, dst, imm->i12);
    else if (width == TC48_OPERAND_WIDTH_24) set_half(regs, dst, imm->i24);
    else if (width == TC48_OPERAND_WIDTH_48) set_word(regs, dst, imm->i48);
}

void tc48_cpu_zero_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst) {
    if      (width == TC48_OPERAND_WIDTH_6)  set_tryte(regs, dst, 0);
    else if (width == TC48_OPERAND_WIDTH_12) set_quarter(regs, dst, 0);
    else if (width == TC48_OPERAND_WIDTH_24) set_half(regs, dst, 0);
    else if (width == TC48_OPERAND_WIDTH_48) set_word(regs, dst, 0);
}

void tc48_cpu_mov_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, tc48_reg_id src) {
    if      (width == TC48_OPERAND_WIDTH_6)  set_tryte(regs, dst, get_tryte(regs, src));
    else if (width == TC48_OPERAND_WIDTH_12) set_quarter(regs, dst, get_quarter(regs, src));
    else if (width == TC48_OPERAND_WIDTH_24) set_half(regs, dst, get_half(regs, src));
    else if (width == TC48_OPERAND_WIDTH_48) set_word(regs, dst, get_word(regs, src));
}

tc48_tryte   tc48_cpu_read_reg6 (tc48_cpu_regs* regs, tc48_reg_id r) { return get_tryte(regs, r); }
tc48_quarter tc48_cpu_read_reg12(tc48_cpu_regs* regs, tc48_reg_id r) { return get_quarter(regs, r); }
tc48_half    tc48_cpu_read_reg24(tc48_cpu_regs* regs, tc48_reg_id r) { return get_half(regs, r); }
tc48_word    tc48_cpu_read_reg48(tc48_cpu_regs* regs, tc48_reg_id r) { return get_word(regs, r); }

#define MATH_IMPL_OP2(op, type) \
    void tc48_cpu_##op##_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2) { \
        set_##type(regs, dst, tc48_math_##type##_##op(get_##type(regs, src1), get_##type(regs, src2))); \
    }

#define MATH_IMPL_OP1(op, type) \
    void tc48_cpu_##op##_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src) { \
        set_##type(regs, dst, tc48_math_##type##_##op(get_##type(regs, src))); \
    }

#define MATH_IMPL_SHIFT(op, type) \
    void tc48_cpu_##op##_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src, int count) { \
        set_##type(regs, dst, tc48_math_##type##_##op(get_##type(regs, src), count)); \
    }

#define MATH_IMPL_TYPE(type) \
    MATH_IMPL_OP2(and, type) MATH_IMPL_OP2(or, type) MATH_IMPL_OP2(xor, type) \
    MATH_IMPL_OP1(not, type) \
    MATH_IMPL_SHIFT(shl, type) MATH_IMPL_SHIFT(shr, type)

MATH_IMPL_TYPE(tryte)
MATH_IMPL_TYPE(quarter)
MATH_IMPL_TYPE(half)
MATH_IMPL_TYPE(word)
