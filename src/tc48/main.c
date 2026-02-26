#include <tc48/system.h>

#include <stdbool.h>

int main() {
    tc48_system sys;
    tc48_system_init(&sys, 729);

    while (true) {
        tc48_system_step(&sys);
    }

    tc48_system_deinit(&sys);
}
