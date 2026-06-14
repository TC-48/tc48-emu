#include <tc48/system.h>

#include <tc48/features.h>
#if TC48_HAS_FEATURE(tva)
    #include <tva/tva.h>
#endif

#include <stdbool.h>

int main(int argc, char** argv) {
#if TC48_HAS_FEATURE(tva)
    tva_global_init();
#endif

    tc48_system sys;
    tc48_system_init(&sys, 729);

    if (argc > 1) {
        tc48_mem_open(sys.bus.mem, argv[1]);
    }

    while (!sys.cpus[0].halted) {
        tc48_system_step(&sys);
    }

    tc48_system_deinit(&sys);

#if TC48_HAS_FEATURE(tva)
    tva_global_deinit();
#endif

    return 1;
}
