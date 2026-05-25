#include <tc48/system.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    tc48_system sys;
    tc48_system_init(&sys, 729);

    if (argc > 1) {
        tc48_mem_load_file(sys.mem, argv[1]);
    }

    while (true) {
        tc48_system_step(&sys);
    }

    tc48_system_deinit(&sys);
}
