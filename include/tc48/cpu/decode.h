#pragma once

#include <tc48/cpu/instr.h>
#include <tc48/bus.h>

// returns how many trytes was consumed by the instruction (or just 0 on error)
tc48_word tc48_decode(tc48_bus* bus, tc48_word addr, tc48_instr* instr);
