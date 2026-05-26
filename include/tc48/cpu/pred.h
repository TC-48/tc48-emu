#pragma once

#include <tc48/cpu/instr.h>
#include <tc48/cpu/regs.h>
#include <tc48/cpu.h>

#include <stdbool.h>

bool tc48_cpu_check_pred(tc48_cpu* cpu, tc48_triplet pred);
