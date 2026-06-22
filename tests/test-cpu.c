#include <criterion/criterion.h>
#include <string.h>

#include <tc48/cpu/encode.h>
#include <tc48/cpu/decode.h>
#include <tc48/cpu/opcode.h>
#include <tc48/mem.h>

static void assert_instr_eq(const tc48_instr* a, const tc48_instr* b) {
    cr_assert_eq(a->format, b->format);
    cr_assert_eq(a->width, b->width);
    cr_assert_eq(a->wcfr, b->wcfr);
    cr_assert_eq(a->pred, b->pred);
    cr_assert_eq(a->opcode, b->opcode);

    switch (a->format) {
    case TC48_INSTR_FORMAT_NONE:
        break;
    case TC48_INSTR_FORMAT_R:
        cr_assert_eq(a->operands.r.r1.base, b->operands.r.r1.base);
        cr_assert_eq(a->operands.r.r1.lane, b->operands.r.r1.lane);
        break;
    case TC48_INSTR_FORMAT_RR:
        cr_assert_eq(a->operands.rr.r1.base, b->operands.rr.r1.base);
        cr_assert_eq(a->operands.rr.r1.lane, b->operands.rr.r1.lane);
        cr_assert_eq(a->operands.rr.r2.base, b->operands.rr.r2.base);
        cr_assert_eq(a->operands.rr.r2.lane, b->operands.rr.r2.lane);
        break;
    case TC48_INSTR_FORMAT_RRR:
        cr_assert_eq(a->operands.rrr.r1.base, b->operands.rrr.r1.base);
        cr_assert_eq(a->operands.rrr.r1.lane, b->operands.rrr.r1.lane);
        cr_assert_eq(a->operands.rrr.r2.base, b->operands.rrr.r2.base);
        cr_assert_eq(a->operands.rrr.r2.lane, b->operands.rrr.r2.lane);
        cr_assert_eq(a->operands.rrr.r3.base, b->operands.rrr.r3.base);
        cr_assert_eq(a->operands.rrr.r3.lane, b->operands.rrr.r3.lane);
        break;
    case TC48_INSTR_FORMAT_RI:
        cr_assert_eq(a->operands.ri.r1.base, b->operands.ri.r1.base);
        cr_assert_eq(a->operands.ri.r1.lane, b->operands.ri.r1.lane);
        if (a->width == TC48_OPERAND_WIDTH_6) cr_assert_eq(a->operands.ri.imm.i6, b->operands.ri.imm.i6);
        else if (a->width == TC48_OPERAND_WIDTH_12) cr_assert_eq(a->operands.ri.imm.i12, b->operands.ri.imm.i12);
        else if (a->width == TC48_OPERAND_WIDTH_24) cr_assert_eq(a->operands.ri.imm.i24, b->operands.ri.imm.i24);
        else if (a->width == TC48_OPERAND_WIDTH_48) cr_assert(a->operands.ri.imm.i48 == b->operands.ri.imm.i48);
        break;
    case TC48_INSTR_FORMAT_RRI:
        cr_assert_eq(a->operands.rri.r1.base, b->operands.rri.r1.base);
        cr_assert_eq(a->operands.rri.r1.lane, b->operands.rri.r1.lane);
        cr_assert_eq(a->operands.rri.r2.base, b->operands.rri.r2.base);
        cr_assert_eq(a->operands.rri.r2.lane, b->operands.rri.r2.lane);
        if (a->width == TC48_OPERAND_WIDTH_6) cr_assert_eq(a->operands.rri.imm.i6, b->operands.rri.imm.i6);
        else if (a->width == TC48_OPERAND_WIDTH_12) cr_assert_eq(a->operands.rri.imm.i12, b->operands.rri.imm.i12);
        else if (a->width == TC48_OPERAND_WIDTH_24) cr_assert_eq(a->operands.rri.imm.i24, b->operands.rri.imm.i24);
        else if (a->width == TC48_OPERAND_WIDTH_48) cr_assert(a->operands.rri.imm.i48 == b->operands.rri.imm.i48);
        break;
    case TC48_INSTR_FORMAT_RRA:
        cr_assert_eq(a->operands.rra.r1.base, b->operands.rra.r1.base);
        cr_assert_eq(a->operands.rra.r1.lane, b->operands.rra.r1.lane);
        cr_assert_eq(a->operands.rra.r2.base, b->operands.rra.r2.base);
        cr_assert_eq(a->operands.rra.r2.lane, b->operands.rra.r2.lane);
        cr_assert_eq(a->operands.rra.addr,    b->operands.rra.addr);
        break;
    case TC48_INSTR_FORMAT_IRR:
        cr_assert_eq(a->operands.irr.r1.base, b->operands.irr.r1.base);
        cr_assert_eq(a->operands.irr.r1.lane, b->operands.irr.r1.lane);
        cr_assert_eq(a->operands.irr.r2.base, b->operands.irr.r2.base);
        cr_assert_eq(a->operands.irr.r2.lane, b->operands.irr.r2.lane);
        if (a->width == TC48_OPERAND_WIDTH_6) cr_assert_eq(a->operands.irr.imm.i6, b->operands.irr.imm.i6);
        else if (a->width == TC48_OPERAND_WIDTH_12) cr_assert_eq(a->operands.irr.imm.i12, b->operands.irr.imm.i12);
        else if (a->width == TC48_OPERAND_WIDTH_24) cr_assert_eq(a->operands.irr.imm.i24, b->operands.irr.imm.i24);
        else if (a->width == TC48_OPERAND_WIDTH_48) cr_assert(a->operands.irr.imm.i48 == b->operands.irr.imm.i48);
        break;
    case TC48_INSTR_FORMAT_IR:
        cr_assert_eq(a->operands.ir.r1.base, b->operands.ir.r1.base);
        cr_assert_eq(a->operands.ir.r1.lane, b->operands.ir.r1.lane);
        if (a->width == TC48_OPERAND_WIDTH_6) cr_assert_eq(a->operands.ir.imm.i6, b->operands.ir.imm.i6);
        else if (a->width == TC48_OPERAND_WIDTH_12) cr_assert_eq(a->operands.ir.imm.i12, b->operands.ir.imm.i12);
        else if (a->width == TC48_OPERAND_WIDTH_24) cr_assert_eq(a->operands.ir.imm.i24, b->operands.ir.imm.i24);
        else if (a->width == TC48_OPERAND_WIDTH_48) cr_assert(a->operands.ir.imm.i48 == b->operands.ir.imm.i48);
        break;
    }
}

static void test_symmetry(const tc48_instr* orig) {
    tc48_memory* mem = tc48_mem_alloc(100);
    tc48_bus bus; tc48_bus_init(&bus, mem);

    tc48_word enc_len = tc48_encode(mem, 10, orig);
    cr_assert(enc_len > 0);

    tc48_instr decoded;
    memset(&decoded, 0, sizeof(decoded));
    tc48_word dec_len = tc48_decode(&bus, 10, &decoded);

    cr_assert_eq(enc_len, dec_len);
    assert_instr_eq(orig, &decoded);

    tc48_mem_free(mem);
}

Test(cpu_encode_decode, format_none) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_NONE,
        .width = TC48_OPERAND_WIDTH_6,
        .wcfr = TC48_WCFR_STAT,
        .pred = TC48_PRED_EQ,
        .opcode = TC48_OP_NOP
    };
    test_symmetry(&instr);
}

Test(cpu_encode_decode, format_r) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_R,
        .width = TC48_OPERAND_WIDTH_12,
        .wcfr = TC48_WCFR_NONE,
        .pred = TC48_PRED_AW,
        .opcode = TC48_OP_HALT,
        .operands.r.r1 = { .base = 12, .lane = 2 }
    };
    test_symmetry(&instr);
}

Test(cpu_encode_decode, format_rrr) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_RRR,
        .width = TC48_OPERAND_WIDTH_6,
        .wcfr = TC48_WCFR_STAT,
        .pred = TC48_PRED_GE,
        .opcode = TC48_OP_SUB,
        .operands.rrr.r1 = { .base = 1, .lane = 0 },
        .operands.rrr.r2 = { .base = 2, .lane = 1 },
        .operands.rrr.r3 = { .base = 3, .lane = 2 }
    };
    test_symmetry(&instr);
}

Test(cpu_encode_decode, format_ri) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_RI,
        .width = TC48_OPERAND_WIDTH_12,
        .wcfr = TC48_WCFR_NONE,
        .pred = TC48_PRED_NZ,
        .opcode = TC48_OP_LOAD,
        .operands.ri.r1 = { .base = 4, .lane = 1 },
        .operands.ri.imm.i12 = 543
    };
    test_symmetry(&instr);
}

Test(cpu_encode_decode, format_rri) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_RRI,
        .width = TC48_OPERAND_WIDTH_24,
        .wcfr = TC48_WCFR_FULL,
        .pred = TC48_PRED_NE,
        .opcode = TC48_OP_STORE,
        .operands.rri.r1 = { .base = 9, .lane = 0 },
        .operands.rri.r2 = { .base = 10, .lane = 2 },
        .operands.rri.imm.i24 = 9999
    };
    test_symmetry(&instr);
}

Test(cpu_encode_decode, format_irr) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_IRR,
        .width = TC48_OPERAND_WIDTH_24,
        .wcfr = TC48_WCFR_FULL,
        .pred = TC48_PRED_NE,
        .opcode = TC48_OP_STORE,
        .operands.irr.r1 = { .base = 9, .lane = 0 },
        .operands.irr.r2 = { .base = 10, .lane = 2 },
        .operands.irr.imm.i24 = 9999
    };
    test_symmetry(&instr);
}

Test(cpu_encode_decode, format_ir) {
    tc48_instr instr = {
        .format = TC48_INSTR_FORMAT_IR,
        .width = TC48_OPERAND_WIDTH_24,
        .wcfr = TC48_WCFR_FULL,
        .pred = TC48_PRED_NE,
        .opcode = TC48_OP_ISTORE,
        .operands.ir.r1 = { .base = 9, .lane = 0 },
        .operands.ir.imm.i24 = 9999
    };
    test_symmetry(&instr);
}
