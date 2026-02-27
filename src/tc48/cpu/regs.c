#include <tc48/cpu/regs.h>

#include <tc48/cpu/instr.h>
#include <tc48/cpu/math.h>

static inline tc48_usize reg_to_idx(tc48_reg_id r) {
    tc48_usize l = r.base.l;
    tc48_usize h = r.base.h;

    tc48_usize d0 = (l & 1) | ((h & 1) << 1);
    tc48_usize d1 = ((l >> 1) & 1) | (((h >> 1) & 1) << 1);
    tc48_usize d2 = ((l >> 2) & 1) | (((h >> 2) & 1) << 1);
    tc48_usize d3 = ((l >> 3) & 1) | (((h >> 3) & 1) << 1);
    return d0 + d1*3 + d2*9 + d3*27;
}
static inline tc48_usize lane_to_off(tc48_reg_id r) {
    tc48_usize l = r.lane.l;
    tc48_usize h = r.lane.h;

    tc48_usize d0 = (l & 1) | ((h & 1) << 1);
    tc48_usize d1 = ((l >> 1) & 1) | (((h >> 1) & 1) << 1);

    return d0 + d1*3;
}

#define GEN_ACCESSORS(name, type, bits, mask)                                                       \
    static type get_##name(tc48_cpu_regs* regs, tc48_reg_id r) {                                    \
        tc48_usize idx = reg_to_idx(r);                                                             \
        tc48_usize off = lane_to_off(r) * bits;                                                     \
        tc48_word w = regs->data[idx];                                                              \
        type res;                                                                                   \
        res.l = (tc48_u64b) ((w.l >> (unsigned) off) & (mask));                                     \
        res.h = (tc48_u64b) ((w.h >> (unsigned) off) & (mask));                                     \
        return res;                                                                                 \
    }                                                                                               \
    static void set_##name(tc48_cpu_regs* regs, tc48_reg_id r, type val) {                          \
        tc48_usize idx = reg_to_idx(r);                                                             \
        tc48_usize off = lane_to_off(r) * bits;                                                     \
        tc48_u64b m = (tc48_u64b) (mask) << (unsigned) off;                                         \
        regs->data[idx].l = (regs->data[idx].l & ~m) | (((tc48_u64b) val.l << (unsigned)off) & m);  \
        regs->data[idx].h = (regs->data[idx].h & ~m) | (((tc48_u64b) val.h << (unsigned)off) & m);  \
    }

GEN_ACCESSORS(tryte,      tc48_tryte,      6,  TC48_TRYTE_MASK)
GEN_ACCESSORS(quarter,    tc48_quarter,    12, TC48_QUARTER_MASK)
GEN_ACCESSORS(half,       tc48_half,       24, TC48_HALF_MASK)
GEN_ACCESSORS(word,       tc48_word,       48, TC48_WORD_MASK)

void tc48_cpu_set_reg_imm(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, tc48_imm* imm) {
    if      (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_6))  set_tryte(regs, dst, imm->i6);
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_12)) set_quarter(regs, dst, imm->i12);
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_24)) set_half(regs, dst, imm->i24);
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_48)) set_word(regs, dst, imm->i48);
}

void tc48_cpu_zero_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst) {
    if      (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_6))  set_tryte(regs, dst, TC48_TRYTE_ZERO);
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_12)) set_quarter(regs, dst, TC48_QUARTER_ZERO);
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_24)) set_half(regs, dst, TC48_HALF_ZERO);
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_48)) set_word(regs, dst, TC48_WORD_ZERO);
}

void tc48_cpu_mov_reg(tc48_cpu_regs* regs, tc48_doublet width, tc48_reg_id dst, tc48_reg_id src) {
    if      (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_6))  set_tryte(regs, dst, get_tryte(regs, src));
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_12)) set_quarter(regs, dst, get_quarter(regs, src));
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_24)) set_half(regs, dst, get_half(regs, src));
    else if (TC48_DOUBLET_EQL(width, TC48_OPERAND_WIDTH_48)) set_word(regs, dst, get_word(regs, src));
}

tc48_tryte   tc48_cpu_read_reg6 (tc48_cpu_regs* regs, tc48_reg_id r) { return get_tryte(regs, r); }
tc48_quarter tc48_cpu_read_reg12(tc48_cpu_regs* regs, tc48_reg_id r) { return get_quarter(regs, r); }
tc48_half    tc48_cpu_read_reg24(tc48_cpu_regs* regs, tc48_reg_id r) { return get_half(regs, r); }
tc48_word    tc48_cpu_read_reg48(tc48_cpu_regs* regs, tc48_reg_id r) { return get_word(regs, r); }

#define ALU_IMPL_OP(op, type) \
    void tc48_cpu_##op##_##type##_reg(tc48_cpu_regs* regs, tc48_reg_id dst, tc48_reg_id src1, tc48_reg_id src2) { \
        set_##type(regs, dst, tc48_math_##type##_##op(get_##type(regs, src1), get_##type(regs, src2))); \
    }

#define ALU_IMPL_TYPE(type) \
    ALU_IMPL_OP(add, type) ALU_IMPL_OP(sub, type) ALU_IMPL_OP(mul, type) ALU_IMPL_OP(div, type) ALU_IMPL_OP(mod, type) \
    ALU_IMPL_OP(sadd, type) ALU_IMPL_OP(ssub, type) ALU_IMPL_OP(smul, type) ALU_IMPL_OP(sdiv, type) ALU_IMPL_OP(smod, type)

ALU_IMPL_TYPE(tryte)
ALU_IMPL_TYPE(quarter)
ALU_IMPL_TYPE(half)
ALU_IMPL_TYPE(word)
