#pragma once
#include <tc48/word.h>
#include <tc48/bus/device.h>
#include <SDL3/SDL.h>

typedef enum {
    TVA_MODE_NONE,
    TVA_MODE_TEXT,
    // TVA_MODE_GRPH,
} tva_mode;

typedef struct {
    tc48_tryte    w, h;
    tc48_quarter  cpos;
    tc48_quarter* cells;
} tva_textbuf;

typedef struct {
    tva_mode mode;
    SDL_Mutex* mutex;
    union {
        tva_textbuf text;
        // tva_grphbuf grph;
    } as;

    SDL_Thread* thread;
    SDL_AtomicInt running;
} tva_dev_state;

bool tva_start_in_bg(tva_dev_state* state);
bool tva_stop_bg(tva_dev_state* state);
