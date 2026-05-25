#pragma once

#include <tc48/cpu/instr.h>
#include <tc48/mem.h>

// returns how many trytes was consumed by the instruction (or just 0 on error)
tc48_word tc48_decode(const tc48_memory* mem, tc48_addr addr, tc48_instr* instr);
