#pragma once

#include <tc48/bus/device.h>

#define TC48_BCONTROL_PORT_COUNT 5

#define TC48_BCONTROL_IN_DEVICE_COUNT   0
#define TC48_BCONTROL_IN_SELECTED_INDEX 1
#define TC48_BCONTROL_IN_SELECTED_TYPE  2
#define TC48_BCONTROL_IN_SELECTED_BASE  3
#define TC48_BCONTROL_IN_SELECTED_PORTS 4

#define TC48_BCONTROL_OUT_SELECT 0

extern const tc48_device_class tc48_bus_controller_class;
