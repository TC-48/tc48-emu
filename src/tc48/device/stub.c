#include <tc48/device/stub.h>
#include <stdio.h>

static tc48_word stub_in(tc48_device* self, tc48_word offset, tc48_width width) {
    return (void)self, (void)offset, (void)width, 1234;
}

static void stub_out(tc48_device* self, tc48_word offset, tc48_width width, tc48_word value) {
    (void)self, (void)offset, (void)width, (void)value, printf("Hello, World!\n");
}

const tc48_device_class tc48_stub_device_class = {
    .name    = "stub-device",
    .type_id = 2846,
    .ports   = 1,

    .init    = NULL,
    .in      = stub_in,
    .out     = stub_out,
    .deinit  = NULL,
};
