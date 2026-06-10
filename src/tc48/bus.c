#include <tc48/bus.h>

void tc48_bus_init(tc48_bus* bus, tc48_memory* mem) {
    bus->mem = mem;
}

void tc48_bus_deinit(tc48_bus* bus) {
    (void) bus;
}

tc48_tryte tc48_bus_load6(tc48_bus* bus, tc48_word addr)    { return tc48_mem_load6(bus->mem, addr);  }
tc48_quarter tc48_bus_load12(tc48_bus* bus, tc48_word addr) { return tc48_mem_load12(bus->mem, addr); }
tc48_half tc48_bus_load24(tc48_bus* bus, tc48_word addr)    { return tc48_mem_load24(bus->mem, addr); }
tc48_word tc48_bus_load48(tc48_bus* bus, tc48_word addr)    { return tc48_mem_load48(bus->mem, addr); }

void tc48_bus_store6(tc48_bus* bus, tc48_word addr, tc48_tryte value)    { tc48_mem_store6(bus->mem, addr, value);  }
void tc48_bus_store12(tc48_bus* bus, tc48_word addr, tc48_quarter value) { tc48_mem_store12(bus->mem, addr, value); }
void tc48_bus_store24(tc48_bus* bus, tc48_word addr, tc48_half value)    { tc48_mem_store24(bus->mem, addr, value); }
void tc48_bus_store48(tc48_bus* bus, tc48_word addr, tc48_word value)    { tc48_mem_store48(bus->mem, addr, value); }

// TODO: implementation
tc48_tryte tc48_bus_in6(tc48_bus* bus, tc48_word addr)    { (void) bus, (void) addr; return 0; }
tc48_quarter tc48_bus_in12(tc48_bus* bus, tc48_word addr) { (void) bus, (void) addr; return 0; }
tc48_half tc48_bus_in24(tc48_bus* bus, tc48_word addr)    { (void) bus, (void) addr; return 0; }
tc48_word tc48_bus_in48(tc48_bus* bus, tc48_word addr)    { (void) bus, (void) addr; return 0; }

void tc48_bus_out6(tc48_bus* bus, tc48_word addr, tc48_tryte value)    { (void) bus, (void) addr, (void) value; }
void tc48_bus_out12(tc48_bus* bus, tc48_word addr, tc48_quarter value) { (void) bus, (void) addr, (void) value; }
void tc48_bus_out24(tc48_bus* bus, tc48_word addr, tc48_half value)    { (void) bus, (void) addr, (void) value; }
void tc48_bus_out48(tc48_bus* bus, tc48_word addr, tc48_word value)    { (void) bus, (void) addr, (void) value; }
