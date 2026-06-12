#pragma once

#include <tc48/bus/device.h>

#include <tc48/word.h>
#include <tc48/mem.h>

// TODO: replace this with a dynamic array
//       (everyone knows that will never happen, so just
//        increase that value if it ever gets too low)
#define TC48_BUS_MAX_DEVICES 64

typedef struct tc48_bus {
    tc48_memory* mem;

    tc48_device devices[TC48_BUS_MAX_DEVICES];
    size_t device_count;
} tc48_bus;

void tc48_bus_init(tc48_bus* bus, tc48_memory* mem);
void tc48_bus_deinit(tc48_bus* bus);

tc48_word tc48_bus_register_device(tc48_bus* bus, const tc48_device_class* cls, tc48_word base);
tc48_word tc48_bus_register_device_auto(tc48_bus* bus, const tc48_device_class* cls);

// memory space (load/store)
tc48_tryte   tc48_bus_load6 (tc48_bus* bus, tc48_word addr);
tc48_quarter tc48_bus_load12(tc48_bus* bus, tc48_word addr);
tc48_half    tc48_bus_load24(tc48_bus* bus, tc48_word addr);
tc48_word    tc48_bus_load48(tc48_bus* bus, tc48_word addr);

void tc48_bus_store6 (tc48_bus* bus, tc48_word addr, tc48_tryte value);
void tc48_bus_store12(tc48_bus* bus, tc48_word addr, tc48_quarter value);
void tc48_bus_store24(tc48_bus* bus, tc48_word addr, tc48_half value);
void tc48_bus_store48(tc48_bus* bus, tc48_word addr, tc48_word value);

// port space (in/out)
tc48_tryte   tc48_bus_in6 (tc48_bus* bus, tc48_word addr);
tc48_quarter tc48_bus_in12(tc48_bus* bus, tc48_word addr);
tc48_half    tc48_bus_in24(tc48_bus* bus, tc48_word addr);
tc48_word    tc48_bus_in48(tc48_bus* bus, tc48_word addr);

void tc48_bus_out6 (tc48_bus* bus, tc48_word addr, tc48_tryte value);
void tc48_bus_out12(tc48_bus* bus, tc48_word addr, tc48_quarter value);
void tc48_bus_out24(tc48_bus* bus, tc48_word addr, tc48_half value);
void tc48_bus_out48(tc48_bus* bus, tc48_word addr, tc48_word value);
