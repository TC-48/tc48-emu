#pragma once

#include <tc48/word.h>
#include <tc48/cpu/instr.h>

typedef struct tc48_device tc48_device;
typedef struct tc48_bus tc48_bus;

// did you know that this syntax is valid in C?
//  int typedef MyTuffType;
// is just equivalent to
//  typedef int MyTuffType;
// why? i don't know, ask the creators of C.
tc48_word typedef tc48_device_in_fn (tc48_device* self, tc48_word offset, tc48_width width);
void      typedef tc48_device_out_fn(tc48_device* self, tc48_word offset, tc48_width width, tc48_word value);
void      typedef tc48_device_deinit_fn(tc48_device* self);
void      typedef *tc48_device_init_fn(tc48_device* self); // this syntax so weird that cdecl refuses to translate it lmao
                                                           // so if you can't read this: return type is void* not void

// yeah, this syntax also works with structs
struct tc48_device_class {
    const char* name;  ///< readable name for debugging
    tc48_word type_id; ///< unique type identifier for discovery
    tc48_word ports;   ///< how many ports this device uses

    // the callbacks
    tc48_device_in_fn*     in;     ///< IN operation handler, not-nullable
    tc48_device_out_fn*    out;    ///< OUT operation handler, not-nullable

    tc48_device_init_fn*   init;   ///< "constructor", nullable
    tc48_device_deinit_fn* deinit; ///< "destructor", nullable
} typedef tc48_device_class;

typedef struct tc48_device {
    const tc48_device_class* class;
    tc48_word base; ///< starting port address
    /// instance-specific state
    struct {
        tc48_bus* bus;
        void*    data;
    } context;
} tc48_device;
