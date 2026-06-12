#include <tc48/system.h>
#include <stdlib.h>

#include <tc48/features.h>
#if TC48_HAS_FEATURE(tva)
#   include <tva/tva.h>
#endif

#include <tc48/device/bcontrol.h>
#include <tc48/device/stub.h>

// NOTE: currently we only step the first CPU as a placeholder
// TODO: add scheduler

void tc48_system_init(tc48_system* sys, size_t mem_size) {
    tc48_memory* mem = tc48_mem_alloc(mem_size);
    tc48_bus_init(&sys->bus, mem);
    tc48_bus_register_device(&sys->bus, &tc48_bus_controller_class, 0);
    tc48_bus_register_device_auto(&sys->bus, &tc48_stub_device_class);

    sys->cpus[0].sys = sys;
    tc48_cpu_init(&sys->cpus[0]);

#if TC48_HAS_FEATURE(tva)
    tva_test();
#endif
}

void tc48_system_deinit(tc48_system* sys) {
    tc48_cpu_deinit(&sys->cpus[0]);
    if (sys->bus.mem) {
        tc48_mem_free(sys->bus.mem);
        sys->bus.mem = NULL;
    }
    tc48_bus_deinit(&sys->bus);
}

void tc48_system_reset(tc48_system* sys) {
    tc48_cpu_reset(&sys->cpus[0]);
}

void tc48_system_step(tc48_system* sys) {
    tc48_cpu_step(&sys->cpus[0]);
}
