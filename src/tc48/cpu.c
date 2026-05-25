#include <tc48/cpu.h>

#include <tc48/cpu/decode.h>
#include <tc48/cpu/opcode.h>
#include <tc48/cpu/regs.h>
#include <tc48/cpu/math.h>

#include <tc48/system.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void tc48_cpu_init(tc48_cpu* cpu) { tc48_cpu_reset(cpu); }
void tc48_cpu_deinit(tc48_cpu* cpu) { (void) cpu; }

void tc48_cpu_reset(tc48_cpu* cpu) {
    memset(&cpu->regs.data, 0, sizeof cpu->regs.data);
}

#define EXEC_RRR_OP(OP)                                                                                                         \
    switch (instr->width) {                                                                                                     \
    case TC48_OPERAND_WIDTH_6:                                                                                                  \
        tc48_cpu_##OP##_tryte_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3); break;   \
    case TC48_OPERAND_WIDTH_12:                                                                                                 \
        tc48_cpu_##OP##_quarter_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3); break; \
    case TC48_OPERAND_WIDTH_24:                                                                                                 \
        tc48_cpu_##OP##_half_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3); break;    \
    case TC48_OPERAND_WIDTH_48:                                                                                                 \
        tc48_cpu_##OP##_word_reg(&cpu->regs, instr->operands.rrr.r1, instr->operands.rrr.r2, instr->operands.rrr.r3); break;    \
    }

#define EXEC_RRI_OP(OP)                                                                                                                    \
    do {                                                                                                                                   \
        tc48_imm res;                                                                                                                      \
        switch (instr->width) {                                                                                                            \
        case TC48_OPERAND_WIDTH_6:                                                                                                         \
            res.i6  = tc48_math_tryte_##OP(tc48_cpu_read_reg6(&cpu->regs, instr->operands.rri.r2), instr->operands.rri.imm.i6); break;     \
        case TC48_OPERAND_WIDTH_12:                                                                                                        \
            res.i12 = tc48_math_quarter_##OP(tc48_cpu_read_reg12(&cpu->regs, instr->operands.rri.r2), instr->operands.rri.imm.i12); break; \
        case TC48_OPERAND_WIDTH_24:                                                                                                        \
            res.i24 = tc48_math_half_##OP(tc48_cpu_read_reg24(&cpu->regs, instr->operands.rri.r2), instr->operands.rri.imm.i24); break;    \
        case TC48_OPERAND_WIDTH_48:                                                                                                        \
            res.i48 = tc48_math_word_##OP(tc48_cpu_read_reg48(&cpu->regs, instr->operands.rri.r2), instr->operands.rri.imm.i48); break;    \
        }                                                                                                                                  \
        tc48_cpu_set_reg_imm(&cpu->regs, instr->width, instr->operands.rri.r1, &res);                                                      \
    } while (0)

#define EXEC_RRI_OR_RRR_OP(INSTR, OP) \
    if ((INSTR)->format == TC48_INSTR_FORMAT_RRR) { \
        EXEC_RRR_OP(OP); \
    } else if ((INSTR)->format == TC48_INSTR_FORMAT_RRI) { \
        EXEC_RRI_OP(OP); \
    } \
    break;

void tc48_cpu_exec(tc48_cpu* cpu, const tc48_instr* instr) {
    printf("format:%d width:%d opcode:%d\n", instr->format, instr->width, instr->opcode);
    switch (instr->opcode) {
    case TC48_OP_HALT: exit(1);
    case TC48_OP_ADD:  EXEC_RRI_OR_RRR_OP(instr, add);
    case TC48_OP_SUB:  EXEC_RRI_OR_RRR_OP(instr, sub);
    }
}

void tc48_cpu_step(tc48_cpu* cpu) {
    tc48_instr instr;
    int off = tc48_decode(cpu->sys->mem, cpu->regs.data[TC48_CPU_REG_PC], &instr);
    if (off == 0) exit(1); // TODO: simplified
    cpu->regs.data[TC48_CPU_REG_PC] += off;

    tc48_cpu_exec(cpu, &instr);
    tc48_cpu_dump_regs(&cpu->regs, stdout);
}

void tc48_cpu_run(tc48_cpu* cpu, tc48_u64b cycles) { (void) cpu, (void) cycles; }
