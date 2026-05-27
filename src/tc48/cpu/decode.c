#include <tc48/cpu/decode.h>
#include <tc48/cpu/opcode.h>
#include <tc48/pow3.h>

// Big-Endian field extraction: start is trit index from MSB (0-based)
static tc48_word _get_val(tc48_word val, int val_width, int start, int count) {
    return (val / tc48_pow3_u128[val_width - start - count]) % tc48_pow3_u128[count];
}

static void _decode_reg(const tc48_memory* mem, tc48_word* addr, tc48_reg_id* reg) {
    tc48_tryte r = tc48_mem_load6(mem, *addr);
    reg->base = (tc48_quadruplet)_get_val(r, TC48_TRYTE_TRITS, 0, 4);
    reg->lane = (tc48_doublet)   _get_val(r, TC48_TRYTE_TRITS, 4, 2);
    *addr += 1;
}

static void _decode_imm(const tc48_memory* mem, tc48_word* addr, tc48_doublet width, tc48_imm* imm) {
    switch (width) {
    case TC48_OPERAND_WIDTH_6:
        imm->i6 = (tc48_tryte)_get_val(tc48_mem_load6(mem, *addr), TC48_TRYTE_TRITS, 0, 6);
        *addr += 1;
        break;
    case TC48_OPERAND_WIDTH_12:
        imm->i12 = (tc48_quarter)_get_val(tc48_mem_load12(mem, *addr), TC48_QUARTER_TRITS, 0, 12);
        *addr += 2;
        break;
    case TC48_OPERAND_WIDTH_24:
        imm->i24 = (tc48_half)_get_val(tc48_mem_load24(mem, *addr), TC48_HALF_TRITS, 0, 24);
        *addr += 4;
        break;
    case TC48_OPERAND_WIDTH_48:
        imm->i48 = (tc48_word)_get_val(tc48_mem_load48(mem, *addr), TC48_WORD_TRITS, 0, 48);
        *addr += 8;
        break;
    }
}

tc48_word tc48_decode(const tc48_memory* mem, tc48_word addr, tc48_instr* instr) {
    tc48_word start_addr = addr;
    tc48_quarter header = tc48_mem_load12(mem, addr);
    addr += 2;

    instr->format = (tc48_doublet)   _get_val(header, TC48_QUARTER_TRITS, 0, 2);
    instr->width  = (tc48_doublet)   _get_val(header, TC48_QUARTER_TRITS, 2, 2);
    instr->wcfr   = (tc48_trit_state)_get_val(header, TC48_QUARTER_TRITS, 4, 1);
    instr->pred   = (tc48_triplet)   _get_val(header, TC48_QUARTER_TRITS, 5, 3);
    instr->opcode = (tc48_quadruplet)_get_val(header, TC48_QUARTER_TRITS, 8, 4);

    switch (instr->format) {
    case TC48_INSTR_FORMAT_NONE:
        break;
    case TC48_INSTR_FORMAT_R:
        _decode_reg(mem, &addr, &instr->operands.r.r1);
        break;
    case TC48_INSTR_FORMAT_I:
        _decode_imm(mem, &addr, instr->width, &instr->operands.i.imm);
        break;
    case TC48_INSTR_FORMAT_RR:
        _decode_reg(mem, &addr, &instr->operands.rr.r1);
        _decode_reg(mem, &addr, &instr->operands.rr.r2);
        break;
    case TC48_INSTR_FORMAT_RRR:
        _decode_reg(mem, &addr, &instr->operands.rrr.r1);
        _decode_reg(mem, &addr, &instr->operands.rrr.r2);
        _decode_reg(mem, &addr, &instr->operands.rrr.r3);
        break;
    case TC48_INSTR_FORMAT_RI:
        _decode_reg(mem, &addr, &instr->operands.ri.r1);
        _decode_imm(mem, &addr, instr->width, &instr->operands.ri.imm);
        break;
    case TC48_INSTR_FORMAT_RRI:
        _decode_reg(mem, &addr, &instr->operands.rri.r1);
        _decode_reg(mem, &addr, &instr->operands.rri.r2);
        _decode_imm(mem, &addr, instr->width, &instr->operands.rri.imm);
        break;
    default:
        return 0;
    }

    return addr - start_addr;
}
