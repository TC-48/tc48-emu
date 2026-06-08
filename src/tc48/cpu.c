#include <tc48/cpu.h>

#include <tc48/cpu/decode.h>
#include <tc48/cpu/opcode.h>
#include <tc48/cpu/regs.h>
#include <tc48/cpu/math.h>
#include <tc48/cpu/pred.h>

#include <tc48/system.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void tc48_cpu_init(tc48_cpu* cpu) { tc48_cpu_reset(cpu); }
void tc48_cpu_deinit(tc48_cpu* cpu) { (void) cpu; }

void tc48_cpu_reset(tc48_cpu* cpu) {
    memset(&cpu->regs.data, 0, sizeof cpu->regs.data);
}

#define EXEC_RRR_OP(OP)                                                                                                                      \
    switch (instr->width) {                                                                                                                  \
    case TC48_OPERAND_WIDTH_6:                                                                                                               \
        tc48_cpu_##OP##_tryte_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3, instr->wcfr); break;   \
    case TC48_OPERAND_WIDTH_12:                                                                                                              \
        tc48_cpu_##OP##_quarter_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3, instr->wcfr); break; \
    case TC48_OPERAND_WIDTH_24:                                                                                                              \
        tc48_cpu_##OP##_half_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3, instr->wcfr); break;    \
    case TC48_OPERAND_WIDTH_48:                                                                                                              \
        tc48_cpu_##OP##_word_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3, instr->wcfr); break;    \
    }

#define EXEC_RRI_OP(OP)                                                                                                                           \
    switch (instr->width) {                                                                                                                       \
    case TC48_OPERAND_WIDTH_6:                                                                                                                    \
        tc48_cpu_##OP##_tryte_imm(&cpu->regs, instr->operands.rri.r1, instr->operands.rri.r2, instr->operands.rri.imm.i6, instr->wcfr); break;    \
    case TC48_OPERAND_WIDTH_12:                                                                                                                   \
        tc48_cpu_##OP##_quarter_imm(&cpu->regs, instr->operands.rri.r1, instr->operands.rri.r2, instr->operands.rri.imm.i12, instr->wcfr); break; \
    case TC48_OPERAND_WIDTH_24:                                                                                                                   \
        tc48_cpu_##OP##_half_imm(&cpu->regs, instr->operands.rri.r1, instr->operands.rri.r2, instr->operands.rri.imm.i24, instr->wcfr); break;    \
    case TC48_OPERAND_WIDTH_48:                                                                                                                   \
        tc48_cpu_##OP##_word_imm(&cpu->regs, instr->operands.rri.r1, instr->operands.rri.r2, instr->operands.rri.imm.i48, instr->wcfr); break;    \
    }                                                                                                                                             \

#define EXEC_RRI_OR_RRR_OP(INSTR, OP)                      \
    if ((INSTR)->format == TC48_INSTR_FORMAT_RRR) {        \
        EXEC_RRR_OP(OP);                                   \
    } else if ((INSTR)->format == TC48_INSTR_FORMAT_RRI) { \
        EXEC_RRI_OP(OP);                                   \
    }                                                      \
    break;

#define EXEC_RR_OP(OP)                                                                                             \
    switch (instr->width) {                                                                                        \
    case TC48_OPERAND_WIDTH_6:                                                                                     \
        tc48_cpu_##OP##_tryte_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr); break;   \
    case TC48_OPERAND_WIDTH_12:                                                                                    \
        tc48_cpu_##OP##_quarter_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr); break; \
    case TC48_OPERAND_WIDTH_24:                                                                                    \
        tc48_cpu_##OP##_half_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr); break;    \
    case TC48_OPERAND_WIDTH_48:                                                                                    \
        tc48_cpu_##OP##_word_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr); break;    \
    }

#define EXEC_RI_OP(OP)                                                                                                  \
    switch (instr->width) {                                                                                             \
    case TC48_OPERAND_WIDTH_6:                                                                                          \
        tc48_cpu_##OP##_tryte_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i6, instr->wcfr); break;    \
    case TC48_OPERAND_WIDTH_12:                                                                                         \
        tc48_cpu_##OP##_quarter_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i12, instr->wcfr); break; \
    case TC48_OPERAND_WIDTH_24:                                                                                         \
        tc48_cpu_##OP##_half_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i24, instr->wcfr); break;    \
    case TC48_OPERAND_WIDTH_48:                                                                                         \
        tc48_cpu_##OP##_word_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i48, instr->wcfr); break;    \
    }

#define EXEC_RI_OR_RR_OP(INSTR, OP)                       \
    if ((INSTR)->format == TC48_INSTR_FORMAT_RR) {        \
        EXEC_RR_OP(OP);                                   \
    } else if ((INSTR)->format == TC48_INSTR_FORMAT_RI) { \
        EXEC_RI_OP(OP);                                   \
    }                                                     \
    break;

static tc48_word sign_extend(tc48_word val, tc48_u128b mod) {
    if (val > mod / 2) {
        return TC48_WORD_VALUES - (mod - val);
    }
    return val;
}

static void update_load_flags(tc48_cpu* cpu, tc48_trit_state wcfr, tc48_word res, tc48_u128b mod) {
    if (wcfr == TC48_WCFR_NONE) return;
    tc48_word* cf = &cpu->regs.data[TC48_CPU_REG_CF];

    tc48_trit_state res_status;
    if (res == 0) res_status = TC48_CF_S_ZERO;
    else if (res <= mod / 2) res_status = TC48_CF_S_POS;
    else res_status = TC48_CF_S_NEG;

    tc48_word_set_trit(cf, TC48_CF_TRIT_S, res_status);
}

#define EXEC_LOAD_OP(INSTR, WIDTH, TYPE, MOD) {                                                                   \
    tc48_reg_id r1 = INSTR->format == TC48_INSTR_FORMAT_RRR ? INSTR->operands.rrr.r1 : INSTR->operands.rra.r1;    \
    tc48_reg_id r2 = INSTR->format == TC48_INSTR_FORMAT_RRR ? INSTR->operands.rrr.r2 : INSTR->operands.rra.r2;    \
    tc48_word off = INSTR->format == TC48_INSTR_FORMAT_RRR                                                        \
            ? tc48_cpu_read_reg48(&cpu->regs, INSTR->operands.rrr.r3)                                             \
            : INSTR->operands.rra.addr;                                                                           \
                                                                                                                  \
    tc48_word addr = tc48_math_word_add(tc48_cpu_read_reg48(&cpu->regs, r2), sign_extend(off, TC48_WORD_VALUES)); \
    tc48_##TYPE res = tc48_mem_load##WIDTH(cpu->sys->mem, addr);                                                  \
    tc48_cpu_write_reg##WIDTH(&cpu->regs, r1, res);                                                               \
    update_load_flags(cpu, INSTR->wcfr, (tc48_word)res, MOD);                                                     \
}

#define EXEC_STORE_OP(INSTR, WIDTH, TYPE, MOD) {                                                                  \
    tc48_reg_id r1 = INSTR->format == TC48_INSTR_FORMAT_RRR ? INSTR->operands.rrr.r1 : INSTR->operands.rra.r1;    \
    tc48_reg_id r2 = INSTR->format == TC48_INSTR_FORMAT_RRR ? INSTR->operands.rrr.r2 : INSTR->operands.rra.r2;    \
    tc48_word off = INSTR->format == TC48_INSTR_FORMAT_RRR                                                        \
            ? tc48_cpu_read_reg48(&cpu->regs, INSTR->operands.rrr.r3)                                             \
            : INSTR->operands.rra.addr;                                                                           \
                                                                                                                  \
    tc48_word addr = tc48_math_word_add(tc48_cpu_read_reg48(&cpu->regs, r2), sign_extend(off, TC48_WORD_VALUES)); \
    tc48_##TYPE val = tc48_cpu_read_reg##WIDTH(&cpu->regs, r1);                                                   \
    tc48_mem_store##WIDTH(cpu->sys->mem, addr, val);                                                              \
}

#define EXEC_MEMORY_OP(INSTR, OP_NAME)                                                                 \
    switch ((INSTR)->width) {                                                                          \
    case TC48_OPERAND_WIDTH_6:  EXEC_##OP_NAME##_OP((INSTR), 6,  tryte,   TC48_TRYTE_VALUES);   break; \
    case TC48_OPERAND_WIDTH_12: EXEC_##OP_NAME##_OP((INSTR), 12, quarter, TC48_QUARTER_VALUES); break; \
    case TC48_OPERAND_WIDTH_24: EXEC_##OP_NAME##_OP((INSTR), 24, half,    TC48_HALF_VALUES);    break; \
    case TC48_OPERAND_WIDTH_48: EXEC_##OP_NAME##_OP((INSTR), 48, word,    TC48_WORD_VALUES);    break; \
    }                                                                                                  \
    break;

void tc48_cpu_exec(tc48_cpu* cpu, const tc48_instr* instr) {
    if (!tc48_cpu_check_pred(cpu, instr->pred)) {
        return;
    }

    printf("format:%d width:%d opcode:%d\n", instr->format, instr->width, instr->opcode);
    switch ((enum tc48_opcode)instr->opcode) {
    case TC48_OP_NOP: break;
    case TC48_OP_HALT: {
        tc48_cpu_dump_regs(&cpu->regs, stdout);
        exit(1);
    }

    case TC48_OP_ADD:  EXEC_RRI_OR_RRR_OP(instr, add);
    case TC48_OP_SUB:  EXEC_RRI_OR_RRR_OP(instr, sub);
    case TC48_OP_UMUL: EXEC_RRI_OR_RRR_OP(instr, umul);
    case TC48_OP_UDIV: EXEC_RRI_OR_RRR_OP(instr, udiv);
    case TC48_OP_SMUL: EXEC_RRI_OR_RRR_OP(instr, smul);
    case TC48_OP_SDIV: EXEC_RRI_OR_RRR_OP(instr, sdiv);

    case TC48_OP_MIN:  EXEC_RRI_OR_RRR_OP(instr, min);
    case TC48_OP_MAX:  EXEC_RRI_OR_RRR_OP(instr, max);
    case TC48_OP_ROT:  EXEC_RRI_OR_RRR_OP(instr, rot);
    case TC48_OP_SHL:  EXEC_RRI_OR_RRR_OP(instr, shl);
    case TC48_OP_SHR:  EXEC_RRI_OR_RRR_OP(instr, shr);

    case TC48_OP_NOT:  EXEC_RI_OR_RR_OP(instr, not);
    case TC48_OP_NEG:  EXEC_RI_OR_RR_OP(instr, neg);

    case TC48_OP_LOAD:  EXEC_MEMORY_OP(instr, LOAD);
    case TC48_OP_STORE: EXEC_MEMORY_OP(instr, STORE);
    }
}

void tc48_cpu_step(tc48_cpu* cpu) {
    tc48_instr instr;
    int off = tc48_decode(cpu->sys->mem, cpu->regs.data[TC48_CPU_REG_IP], &instr);
    if (off == 0) exit(1); // TODO: simplified
    cpu->regs.data[TC48_CPU_REG_IP] += off;

    tc48_cpu_exec(cpu, &instr);
}

void tc48_cpu_run(tc48_cpu* cpu, tc48_u64b cycles) { (void) cpu, (void) cycles; }
