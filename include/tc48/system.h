#pragma once

#include <tc48/cpu.h>
#include <tc48/mem.h>

typedef struct tc48_system {
    tc48_cpu cpus[256];
    tc48_memory mem;
} tc48_system;
