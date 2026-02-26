#include <tc48/system.h>
#include <stdlib.h>

// NOTE: currently we only step the first CPU as a placeholder
// TODO: add scheduler

void tc48_system_init(tc48_system* sys, size_t mem_size) {
    sys->mem = tc48_mem_alloc(mem_size);
    sys->cpus[0].sys = sys;
    tc48_cpu_init(&sys->cpus[0]);
}

void tc48_system_deinit(tc48_system* sys) {
    tc48_cpu_deinit(&sys->cpus[0]);
    if (sys->mem) {
        tc48_mem_free(sys->mem);
        sys->mem = NULL;
    }
}

void tc48_system_reset(tc48_system* sys) {
    tc48_cpu_reset(&sys->cpus[0]);
}

void tc48_system_step(tc48_system* sys) {
    tc48_cpu_step(&sys->cpus[0]);
}
