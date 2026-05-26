#include <tc48/cpu/regs.h>

#include <tc48/cpu/instr.h>
#include <tc48/cpu/math.h>

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

void tc48_cpu_dump_regs(tc48_cpu_regs* regs, FILE* out) {
    for (unsigned i = 0; i < TC48_CPU_REGS_COUNT; ++i) {
        char name[4];
        if      (i == TC48_CPU_REG_PC)  strcpy(name, "pc");
        else if (i == TC48_CPU_REG_CPS) strcpy(name, "cps");
        else if (i == TC48_CPU_REG_AZ)  continue;
        else {
            snprintf(name, sizeof name, "r%u", i-3);
        }

        // TODO: implement printing the whole 128-bit value
        fprintf(out, "%s = %"PRIu64"\n", name, (tc48_u64b)regs->data[i]);

        for (unsigned h = 0; h < 2; ++h) {
            tc48_half value = tc48_cpu_read_reg24(regs, (tc48_reg_id) { .base = i, .lane = h });
            fprintf(out, "  %s:h%u = %"PRIu64"\n", name, h, value);

            for (unsigned q = 0; q < 2; ++q) {
                const unsigned q_lane = h * 2 + q;
                tc48_quarter value = tc48_cpu_read_reg12(regs, (tc48_reg_id) { .base = i, .lane = q_lane });
                fprintf(out, "    %s:q%u = %"PRIu32"\n", name, q_lane, value);

                for (unsigned t = 0; t < 2; ++t) {
                    const unsigned t_lane = q_lane * 2 + t;
                    tc48_tryte value = tc48_cpu_read_reg6(regs, (tc48_reg_id) { .base = i, .lane = t_lane });
                    fprintf(out, "      %s:t%u = %"PRIu16"\n", name, t_lane, value);
                }
            }
        }
    }
}

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
    MATH_IMPL_OP2(min, type) MATH_IMPL_OP2(max, type) \
    MATH_IMPL_OP2(add, type) MATH_IMPL_OP2(sub, type) \
    MATH_IMPL_OP2(umul, type) MATH_IMPL_OP2(udiv, type) \
    MATH_IMPL_OP2(smul, type) MATH_IMPL_OP2(sdiv, type) \
    MATH_IMPL_OP1(not, type) \
    MATH_IMPL_SHIFT(shl, type) MATH_IMPL_SHIFT(shr, type)

MATH_IMPL_TYPE(tryte);
MATH_IMPL_TYPE(quarter);
MATH_IMPL_TYPE(half);
MATH_IMPL_TYPE(word);
