#include <tc48/cpu/encode.h>
#include <tc48/cpu/opcode.h>
#include <tc48/pow3.h>

static void _set_val(tc48_word* val, int val_width, int start, int count, tc48_word field_val) {
    tc48_u128b p_offset = tc48_pow3_u128[val_width - start - count];
    tc48_u128b p_count = tc48_pow3_u128[count];
    *val = *val - ((*val / p_offset) % p_count) * p_offset + (field_val % p_count) * p_offset;
}

static void _encode_reg(tc48_memory* mem, tc48_word* addr, const tc48_reg_id* reg) {
    tc48_word r = 0;
    _set_val(&r, TC48_TRYTE_TRITS, 0, 4, reg->base);
    _set_val(&r, TC48_TRYTE_TRITS, 4, 2, reg->lane);
    tc48_mem_store6(mem, *addr, (tc48_tryte)r);
    *addr += 1;
}

static void _encode_imm(tc48_memory* mem, tc48_word* addr, tc48_doublet width, const tc48_imm* imm) {
    tc48_word v = 0;
    switch (width) {
    case TC48_OPERAND_WIDTH_6:
        _set_val(&v, TC48_TRYTE_TRITS, 0, 6, imm->i6);
        tc48_mem_store6(mem, *addr, (tc48_tryte)v);
        *addr += 1;
        break;
    case TC48_OPERAND_WIDTH_12:
        _set_val(&v, TC48_QUARTER_TRITS, 0, 12, imm->i12);
        tc48_mem_store12(mem, *addr, (tc48_quarter)v);
        *addr += 2;
        break;
    case TC48_OPERAND_WIDTH_24:
        _set_val(&v, TC48_HALF_TRITS, 0, 24, imm->i24);
        tc48_mem_store24(mem, *addr, (tc48_half)v);
        *addr += 4;
        break;
    case TC48_OPERAND_WIDTH_48:
        _set_val(&v, TC48_WORD_TRITS, 0, 48, imm->i48);
        tc48_mem_store48(mem, *addr, (tc48_word)v);
        *addr += 8;
        break;
    }
}

static void _encode_addr(tc48_memory* mem, tc48_word* addr, tc48_word in_addr) {
    tc48_mem_store48(mem, *addr, in_addr);
    *addr += 8;
}

tc48_word tc48_encode(tc48_memory* mem, tc48_word addr, const tc48_instr* instr) {
    tc48_word start_addr = addr;

    tc48_word header = 0;
    _set_val(&header, TC48_QUARTER_TRITS, 0, 2, instr->format);
    _set_val(&header, TC48_QUARTER_TRITS, 2, 2, instr->width);
    _set_val(&header, TC48_QUARTER_TRITS, 4, 1, instr->wcfr);
    _set_val(&header, TC48_QUARTER_TRITS, 5, 3, instr->pred);
    _set_val(&header, TC48_QUARTER_TRITS, 8, 4, instr->opcode);

    tc48_mem_store12(mem, addr, (tc48_quarter)header);
    addr += 2;

    switch (instr->format) {
    case TC48_INSTR_FORMAT_NONE:
        break;
    case TC48_INSTR_FORMAT_R:
        _encode_reg(mem, &addr, &instr->operands.r.r1);
        break;
    case TC48_INSTR_FORMAT_RR:
        _encode_reg(mem, &addr, &instr->operands.rr.r1);
        _encode_reg(mem, &addr, &instr->operands.rr.r2);
        break;
    case TC48_INSTR_FORMAT_RRR:
        _encode_reg(mem, &addr, &instr->operands.rrr.r1);
        _encode_reg(mem, &addr, &instr->operands.rrr.r2);
        _encode_reg(mem, &addr, &instr->operands.rrr.r3);
        break;
    case TC48_INSTR_FORMAT_RI:
        _encode_reg(mem, &addr, &instr->operands.ri.r1);
        _encode_imm(mem, &addr, instr->width, &instr->operands.ri.imm);
        break;
    case TC48_INSTR_FORMAT_RRI:
        _encode_reg(mem, &addr, &instr->operands.rri.r1);
        _encode_reg(mem, &addr, &instr->operands.rri.r2);
        _encode_imm(mem, &addr, instr->width, &instr->operands.rri.imm);
        break;
    case TC48_INSTR_FORMAT_RRA:
        _encode_reg(mem, &addr, &instr->operands.rra.r1);
        _encode_reg(mem, &addr, &instr->operands.rra.r2);
        _encode_addr(mem, &addr, instr->operands.rra.addr);
        break;
    case TC48_INSTR_FORMAT_IRR:
        _encode_imm(mem, &addr, instr->width, &instr->operands.irr.imm);
        _encode_reg(mem, &addr, &instr->operands.irr.r1);
        _encode_reg(mem, &addr, &instr->operands.irr.r2);
        break;
    case TC48_INSTR_FORMAT_IR:
        _encode_imm(mem, &addr, instr->width, &instr->operands.ir.imm);
        _encode_reg(mem, &addr, &instr->operands.ir.r1);
        break;
    default:
        return 0;
    }

    return addr - start_addr;
}
