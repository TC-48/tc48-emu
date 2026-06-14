#pragma once

#include <tc48/bus/device.h>

void tva_global_init();
void tva_global_deinit();

#define TVA_PORTS_COUNT 6

#define TVA_OUT_SETUP_TEXT 0
#define TVA_OUT_SETUP_GRPH 1
// text mode
    #define TVA_TEXT_OUT_SET_CPOS 3
    #define TVA_TEXT_OUT_SET_CELL 4
// grph mode
    // TODO: graphics mode not implemented yet

#define TVA_IN_MAX_SIZE_TEXT 0
#define TVA_IN_MAX_SIZE_GRPH 1
// text mode
    #define TVA_TEXT_IN_GET_CPOS 3
    #define TVA_TEXT_IN_GET_CELL 4
    #define TVA_TEXT_IN_GET_SIZE 5

extern tc48_device_class tva_device_class;
