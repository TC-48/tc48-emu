#include <tc48/device/bcontrol.h>
#include <tc48/bus.h>

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    tc48_word select_index;
} tc48_bcontrol_state;

static void* bus_controller_init(tc48_device* self) {
    (void) self;

    tc48_bcontrol_state* state = malloc(sizeof(tc48_bcontrol_state));
    if (state != NULL) {
        state->select_index = 0;
    }
    return state;
}

static tc48_word bus_controller_in(tc48_device* self, tc48_word offset, tc48_width width) {
    (void) width;

    tc48_bcontrol_state* state = self->context.data;
    tc48_bus* bus = self->context.bus;
    bool is_index_valid = state->select_index < bus->device_count;

    switch (offset) {
    case TC48_BCONTROL_IN_DEVICE_COUNT:
        return (tc48_word)bus->device_count;
    case TC48_BCONTROL_IN_SELECT_INDEX:
        return state->select_index;

    case TC48_BCONTROL_IN_SELECTED_TYPE:
        return is_index_valid ? bus->devices[state->select_index].class->type_id : 0;
    case TC48_BCONTROL_IN_SELECTED_BASE:
        return is_index_valid ? bus->devices[state->select_index].base: 0;
    case TC48_BCONTROL_IN_SELECTED_PORTS:
        return is_index_valid ? bus->devices[state->select_index].class->ports : 0;

    default:
        // probably unreachable
        return 0;
    }
}

static void bus_controller_out(tc48_device* self, tc48_word offset, tc48_width width, tc48_word value) {
    (void) width;

    tc48_bcontrol_state* state = self->context.data;
    switch (offset) {
    case TC48_BCONTROL_OUT_SELECT:
        state->select_index = value;
        break;
    }
}

static void bus_controller_deinit(tc48_device* self) {
    if (self->context.data != NULL) {
        free(self->context.data);
    }
}

const tc48_device_class tc48_bus_controller_class = {
    .name    = "bus-controller",
    .type_id = 1223, // TODO: replace this random number with sth like "bcontrol" encoded in TSCS
    .ports   = TC48_BCONTROL_PORT_COUNT,

    .in      = bus_controller_in,
    .out     = bus_controller_out,
    .init    = bus_controller_init,
    .deinit  = bus_controller_deinit,
};
