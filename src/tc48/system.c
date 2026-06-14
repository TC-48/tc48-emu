#include <tc48/system.h>
#include <stdlib.h>

#include <tc48/features.h>
#if TC48_HAS_FEATURE(tva)
#   include <tva/tva.h>
#   include <SDL3/SDL.h>
#endif

#include <tc48/device/bcontrol.h>
#include <tc48/device/stub.h>

// NOTE: currently we only step the first CPU as a placeholder
// TODO: add scheduler

#if TC48_HAS_FEATURE(tva)
    typedef struct {
        tc48_system* sys;
        void* tva_state;
    } cpu_thread_args;

    static int SDLCALL cpu_thread_func(void* data) {
        cpu_thread_args* args = data;
        tc48_system* sys = args->sys;
        void* state = args->tva_state;

        while (!sys->cpus[0].halted && tva_is_running(state)) {
            tc48_system_step(sys);
        }

        tva_stop(state);
        return 0;
    }
#endif

void tc48_system_init(tc48_system* sys, size_t mem_size) {
    tc48_memory* mem = tc48_mem_alloc(mem_size);
    tc48_bus_init(&sys->bus, mem);
    tc48_bus_register_device(&sys->bus, &tc48_bus_controller_class, 0);
    tc48_bus_register_device_auto(&sys->bus, &tc48_stub_device_class);
#if TC48_HAS_FEATURE(tva)
    tc48_bus_register_device_auto(&sys->bus, &tva_device_class);
#endif

    sys->cpus[0].sys = sys;
    tc48_cpu_init(&sys->cpus[0]);
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

void tc48_system_run(tc48_system* sys) {
#if TC48_HAS_FEATURE(tva)
    void* tva_state = NULL;
    for (size_t i = 0; i < sys->bus.device_count; i++) {
        if (sys->bus.devices[i].class == &tva_device_class) {
            tva_state = sys->bus.devices[i].context.data;
            break;
        }
    }

    if (tva_state != NULL) {
        cpu_thread_args args = { sys, tva_state };
        SDL_Thread* cpu_thread = SDL_CreateThread(cpu_thread_func, "TC48-CPU", &args);

        if (cpu_thread != NULL) {
            tva_loop(tva_state);
            SDL_WaitThread(cpu_thread, NULL);
            return;
        }
    }
#endif

    while (!sys->cpus[0].halted) {
        tc48_system_step(sys);
    }
}

