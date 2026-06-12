#include <tc48/bus.h>

#include <assert.h>
#include <stdbool.h>
#include <string.h>

void tc48_bus_init(tc48_bus* bus, tc48_memory* mem) {
    bus->mem = mem;
    bus->device_count = 0;
}

void tc48_bus_deinit(tc48_bus* bus) {
    for (tc48_usize i = 0; i < bus->device_count; i++) {
        tc48_device* dev = &bus->devices[i];
        if (dev->class->deinit != NULL) {
            dev->class->deinit(dev);
        }
    }
}

tc48_tryte tc48_bus_load6(tc48_bus* bus, tc48_word addr)    { return tc48_mem_load6(bus->mem, addr);  }
tc48_quarter tc48_bus_load12(tc48_bus* bus, tc48_word addr) { return tc48_mem_load12(bus->mem, addr); }
tc48_half tc48_bus_load24(tc48_bus* bus, tc48_word addr)    { return tc48_mem_load24(bus->mem, addr); }
tc48_word tc48_bus_load48(tc48_bus* bus, tc48_word addr)    { return tc48_mem_load48(bus->mem, addr); }

void tc48_bus_store6(tc48_bus* bus, tc48_word addr, tc48_tryte value)    { tc48_mem_store6(bus->mem, addr, value);  }
void tc48_bus_store12(tc48_bus* bus, tc48_word addr, tc48_quarter value) { tc48_mem_store12(bus->mem, addr, value); }
void tc48_bus_store24(tc48_bus* bus, tc48_word addr, tc48_half value)    { tc48_mem_store24(bus->mem, addr, value); }
void tc48_bus_store48(tc48_bus* bus, tc48_word addr, tc48_word value)    { tc48_mem_store48(bus->mem, addr, value); }

static tc48_word _find_next_free_port(tc48_bus* bus) {
    tc48_word next_base = 0;
    for (tc48_usize i = 0; i < bus->device_count; i++) {
        tc48_device* existing = &bus->devices[i];
        tc48_word end = existing->base + existing->class->ports;
        if (end > next_base) {
            next_base = end;
        }
    }
    return next_base;
}

tc48_word tc48_bus_register_device(tc48_bus* bus, const tc48_device_class* cls, tc48_word base) {
    assert(bus->device_count < TC48_BUS_MAX_DEVICES);

    tc48_device* dev = &bus->devices[bus->device_count++];
    dev->class = cls;
    dev->base = base;

    dev->context.bus = bus;
    if (cls->init != NULL)
        dev->context.data = cls->init(dev);
    else
        dev->context.data = NULL;

    return base;
}

tc48_word tc48_bus_register_device_auto(tc48_bus* bus, const tc48_device_class* cls) {
    return tc48_bus_register_device(bus, cls, _find_next_free_port(bus));
}

static tc48_device* _find_device(tc48_bus* bus, tc48_word addr) {
    for (tc48_usize i = 0; i < bus->device_count; i++) {
        tc48_device* dev = &bus->devices[i];
        if (addr >= dev->base && addr < dev->base + dev->class->ports) {
            return dev;
        }
    }
    return NULL;
}

#define BUS_PMIO_IMPL(WIDTH, TRITS)                                                                 \
    tc48_##WIDTH tc48_bus_in##TRITS(tc48_bus* bus, tc48_word addr) {                                \
        tc48_device* dev = _find_device(bus, addr);                                                 \
        if (dev != NULL)                                                                            \
            return (tc48_##WIDTH)dev->class->in(dev, addr - dev->base, TC48_OPERAND_WIDTH_##TRITS); \
        return 0;                                                                                   \
    }                                                                                               \
                                                                                                    \
    void tc48_bus_out##TRITS(tc48_bus* bus, tc48_word addr, tc48_##WIDTH value) {                   \
        tc48_device* dev = _find_device(bus, addr);                                                 \
        if (dev != NULL)                                                                            \
            dev->class->out(dev, addr - dev->base, TC48_OPERAND_WIDTH_##TRITS, (tc48_word)value);   \
    }

BUS_PMIO_IMPL(tryte,   6);
BUS_PMIO_IMPL(quarter, 12);
BUS_PMIO_IMPL(half,    24);
BUS_PMIO_IMPL(word,    48);
