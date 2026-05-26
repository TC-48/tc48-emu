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

static inline void tc48_cpu_exec_not_rr(tc48_cpu* cpu, const tc48_instr* instr) {
    switch (instr->width) {
    case TC48_OPERAND_WIDTH_6:
        tc48_cpu_not_tryte_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr);
        break;
    case TC48_OPERAND_WIDTH_12:
        tc48_cpu_not_quarter_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr);
        break;
    case TC48_OPERAND_WIDTH_24:
        tc48_cpu_not_half_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr);
        break;
    case TC48_OPERAND_WIDTH_48:
        tc48_cpu_not_word_reg(&cpu->regs, instr->operands.rr.r1, instr->operands.rr.r2, instr->wcfr);
        break;
    }
}

static inline void tc48_cpu_exec_not_ri(tc48_cpu* cpu, const tc48_instr* instr) {
    switch (instr->width) {
    case TC48_OPERAND_WIDTH_6:
        tc48_cpu_not_tryte_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i6, instr->wcfr);
        break;
    case TC48_OPERAND_WIDTH_12:
        tc48_cpu_not_quarter_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i12, instr->wcfr);
        break;
    case TC48_OPERAND_WIDTH_24:
        tc48_cpu_not_half_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i24, instr->wcfr);
        break;
    case TC48_OPERAND_WIDTH_48:
        tc48_cpu_not_word_imm(&cpu->regs, instr->operands.ri.r1, instr->operands.ri.imm.i48, instr->wcfr);
        break;
    }
}

void tc48_cpu_exec(tc48_cpu* cpu, const tc48_instr* instr) {
    if (!tc48_cpu_check_pred(cpu, instr->pred)) {
        return;
    }

    printf("format:%d width:%d opcode:%d\n", instr->format, instr->width, instr->opcode);
    switch (instr->opcode) {
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

    case TC48_OP_NOT:
        if (instr->format == TC48_INSTR_FORMAT_RR) {
            tc48_cpu_exec_not_rr(cpu, instr);
        } else if (instr->format == TC48_INSTR_FORMAT_RI) {
            tc48_cpu_exec_not_ri(cpu, instr);
        }
        break;

    case TC48_OP_JMP:
        // TODO: i dont know if we should support other operand widths or just enforce 48 trits for the address
        if (instr->format == TC48_INSTR_FORMAT_I) {
            cpu->regs.data[TC48_CPU_REG_PC] = instr->operands.i.imm.i48;
        } else if (instr->format == TC48_INSTR_FORMAT_R) {
            cpu->regs.data[TC48_CPU_REG_PC] = tc48_cpu_read_reg48(&cpu->regs, instr->operands.r.r1);
        }
        break;
    }
}

void tc48_cpu_step(tc48_cpu* cpu) {
    tc48_instr instr;
    int off = tc48_decode(cpu->sys->mem, cpu->regs.data[TC48_CPU_REG_PC], &instr);
    if (off == 0) exit(1); // TODO: simplified
    cpu->regs.data[TC48_CPU_REG_PC] += off;

    tc48_cpu_exec(cpu, &instr);
}

void tc48_cpu_run(tc48_cpu* cpu, tc48_u64b cycles) { (void) cpu, (void) cycles; }
