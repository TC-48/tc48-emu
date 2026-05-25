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

void tc48_cpu_step(tc48_cpu* cpu) {
    tc48_instr instr;
    int off = tc48_decode(cpu->sys->mem, cpu->regs.data[TC48_CPU_REG_PC], &instr);
    if (off == 0) exit(1); // TODO: simplified
    printf("format:%d width:%d opcode:%d\n", instr.format, instr.width, instr.opcode);
    cpu->regs.data[TC48_CPU_REG_PC] += off;

    if (instr.opcode == TC48_OP_HALT) exit(1);
}

void tc48_cpu_run(tc48_cpu* cpu, tc48_u64b cycles) { (void) cpu, (void) cycles; }
