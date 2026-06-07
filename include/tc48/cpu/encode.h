#pragma once

#include <tc48/cpu/instr.h>
#include <tc48/mem.h>

// returns how many trytes were written by the instruction (or 0 on error)
tc48_word tc48_encode(tc48_memory* mem, tc48_word addr, const tc48_instr* instr);
