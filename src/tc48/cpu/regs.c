#include <tc48/cpu/regs.h>

#include <tc48/cpu/instr.h>
#include <tc48/cpu/math.h>
#include <tc48/cpu/opcode.h>

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#define GET_STATUS(RES, MOD)             \
    (RES == 0 ? TC48_CF_S_ZERO :         \
        RES <= (MOD)/2 ? TC48_CF_S_POS   \
            : TC48_CF_S_NEG)

static void update_cf(
    tc48_cpu_regs* regs, tc48_trit_state wcfr, 
    tc48_word res, tc48_word a, tc48_word b, 
    tc48_u128b mod, int op
) {
    if (wcfr == TC48_WCFR_NONE) return;
    tc48_word* cf = &regs->data[TC48_CPU_REG_CF];

    tc48_trit_state res_status = GET_STATUS(res, mod);
    tc48_word_set_trit(cf, TC48_CF_TRIT_S, res_status);

    if (wcfr != TC48_WCFR_FULL) return;

    tc48_trit_state c = TC48_CF_C_NONE, v = TC48_CF_V_NONE;
    tc48_trit_state a_status = GET_STATUS(a, mod);
    tc48_trit_state b_status = GET_STATUS(b, mod);

    switch (op) {
    case TC48_OP_ADD:
        if (res < a && b != 0) c = TC48_CF_C_CARRY;

        // result of adding two positive numbers is always positive (same for negative)
        if (a_status == TC48_CF_S_POS && b_status == TC48_CF_S_POS && res_status != TC48_CF_S_POS) v = TC48_CF_V_POS;
        if (a_status == TC48_CF_S_NEG && b_status == TC48_CF_S_NEG && res_status != TC48_CF_S_NEG) v = TC48_CF_V_NEG;
        break;

    case TC48_OP_SUB:
        if (res > a && b != 0) c = TC48_CF_C_BORROW;

        // pos - neg should be positive, neg - pos should be negative
        if (a_status == TC48_CF_S_POS && b_status == TC48_CF_S_NEG && res_status != TC48_CF_S_POS) v = TC48_CF_V_POS;
        if (a_status == TC48_CF_S_NEG && b_status == TC48_CF_S_POS && res_status != TC48_CF_S_NEG) v = TC48_CF_V_NEG;
        break;

    case TC48_OP_SMUL: {
        const tc48_i128b a_signed = (a <= mod/2) ? a : (tc48_i128b)a - mod;
        const tc48_i128b b_signed = (b <= mod/2) ? b : (tc48_i128b)b - mod;
        const tc48_i128b the_maximum = mod/2;

        if      (a_signed * b_signed > the_maximum)  v = TC48_CF_V_POS;
        else if (a_signed * b_signed < -the_maximum) v = TC48_CF_V_NEG;
        break;
    }
    case TC48_OP_UMUL:
        if (a != 0 && (tc48_u128b)res / a != b) c = TC48_CF_C_CARRY;
        break; 
    }
    tc48_word_set_trit(cf, TC48_CF_TRIT_C, c);
    tc48_word_set_trit(cf, TC48_CF_TRIT_V, v);
}

void tc48_cpu_dump_regs(tc48_cpu_regs* regs, FILE* out) {
    for (unsigned i = 0; i < TC48_CPU_REGS_COUNT; ++i) {
        char name[4];
        if      (i == TC48_CPU_REG_PC) strcpy(name, "pc");
        else if (i == TC48_CPU_REG_CF) strcpy(name, "cf");
        else if (i == TC48_CPU_REG_AZ) continue;
        else {
            snprintf(name, sizeof name, "r%u", i-3);
        }

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

static inline tc48_usize reg_to_idx(tc48_reg_id r) { return (tc48_usize)r.base; }
static inline tc48_usize lane_to_off(tc48_reg_id r) { return (tc48_usize)r.lane; }

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

#define MATH_IMPL_OP2(op, OP, type, mod)                                                                                                \
    void tc48_cpu_##op##_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2, tc48_trit_state wcfr) { \
        tc48_##type a = get_##type(regs, src1), b = get_##type(regs, src2);                                                             \
        tc48_##type res = tc48_math_##type##_##op(a, b);                                                                                \
        set_##type(regs, dst, res);                                                                                                     \
        update_cf(regs, wcfr, (tc48_word)res, (tc48_word)a, (tc48_word)b, mod, TC48_OP_##OP);                                           \
    }                                                                                                                                   \
    void tc48_cpu_##op##_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_##type imm, tc48_trit_state wcfr) {  \
        tc48_##type a = get_##type(regs, src1);                                                                                         \
        tc48_##type res = tc48_math_##type##_##op(a, imm);                                                                              \
        set_##type(regs, dst, res);                                                                                                     \
        update_cf(regs, wcfr, (tc48_word)res, (tc48_word)a, (tc48_word)imm, mod, TC48_OP_##OP);                                         \
    }

#define MATH_IMPL_OP1(op, OP, type, mod)                                                                                                \
    void tc48_cpu_##op##_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src, tc48_trit_state wcfr) {                    \
        tc48_##type a = get_##type(regs, src);                                                                                          \
        tc48_##type res = tc48_math_##type##_##op(a);                                                                                   \
        set_##type(regs, dst, res);                                                                                                     \
        update_cf(regs, wcfr, (tc48_word)res, (tc48_word)a, 0, mod, TC48_OP_##OP);                                                      \
    }

#define MATH_IMPL_OP1_IMM(op, OP, type, mod)                                                                                            \
    void tc48_cpu_##op##_##type##_imm(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_##type imm, tc48_trit_state wcfr) {                    \
        tc48_##type a = imm;                                                                                                            \
        tc48_##type res = tc48_math_##type##_##op(a);                                                                                   \
        set_##type(regs, dst, res);                                                                                                     \
        update_cf(regs, wcfr, (tc48_word)res, (tc48_word)a, 0, mod, TC48_OP_##OP);                                                      \
    }

#define MATH_IMPL_TYPE(type, mod)                                                                                                       \
    MATH_IMPL_OP2(min, MIN, type, mod) MATH_IMPL_OP2(max, MAX, type, mod)                                                               \
    MATH_IMPL_OP2(add, ADD, type, mod) MATH_IMPL_OP2(sub, SUB, type, mod)                                                               \
    MATH_IMPL_OP2(umul, UMUL, type, mod) MATH_IMPL_OP2(udiv, UDIV, type, mod)                                                           \
    MATH_IMPL_OP2(smul, SMUL, type, mod) MATH_IMPL_OP2(sdiv, SDIV, type, mod)                                                           \
    MATH_IMPL_OP2(rot, ROT, type, mod)                                                                                                  \
    MATH_IMPL_OP1(not, NOT, type, mod)                                                                                                  \
    MATH_IMPL_OP1_IMM(not, NOT, type, mod)                                                                                              \
    MATH_IMPL_OP2(shl, SHL, type, mod) MATH_IMPL_OP2(shr, SHR, type, mod)

MATH_IMPL_TYPE(tryte,   TC48_TRYTE_VALUES);
MATH_IMPL_TYPE(quarter, TC48_QUARTER_VALUES);
MATH_IMPL_TYPE(half,    TC48_HALF_VALUES);
MATH_IMPL_TYPE(word,    TC48_WORD_VALUES);
