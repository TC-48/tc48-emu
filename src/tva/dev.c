#include <tva/tva.h>
#include "internal.h"

#include <tc48/word.h>
#include <tc48/macros.h>

#include <stdlib.h>

#define TEXT_MAX_SIZE 260

void* tva_dev_init(tc48_device* self) {
    (void) self;
    tva_dev_state* state = malloc(sizeof(tva_dev_state));
    if (state != NULL) {
        state->mode = TVA_MODE_NONE;
        state->mutex = SDL_CreateMutex();
        tva_start_in_bg(state);
    }
    return state;
}
void tva_dev_deinit(tc48_device* self) {
    tva_dev_state* state = self->context.data;
    tva_stop_bg(state);

    switch (state->mode) {
    case TVA_MODE_NONE: break;
    case TVA_MODE_TEXT:
        free(state->as.text.cells);
    }
    SDL_DestroyMutex(state->mutex);
    free(state);
}

//////////////////////////////////////////////// OUT /////////////////////////////////////////////////
static void tva_text_mode_out(tva_textbuf* t, tc48_word offset, tc48_width width, tc48_word value) {
    switch (offset) {
    case TVA_TEXT_OUT_SET_CELL: {
        tc48_tryte ch, color;
        if (width == TC48_OPERAND_WIDTH_6) {
            ch    = (tc48_tryte)value;
            color = TC48_TRYTE(0,0,0, 0,0,0);
        } else {
            ch    = (tc48_tryte)((tc48_quarter)value / TC48_TRYTE_VALUES);
            color = (tc48_tryte)((tc48_quarter)value % TC48_TRYTE_VALUES);
        }

        t->cells[t->cpos] = ((tc48_quarter)color * TC48_TRYTE_VALUES) + ch;
        break;
    }
    case TVA_TEXT_OUT_SET_CPOS:
        t->cpos = (tc48_quarter)value;
        break;
    }
}

static void tva_dev_out(tc48_device* self, tc48_word offset, tc48_width width, tc48_word value) {
    tva_dev_state* state = self->context.data;
    SDL_LockMutex(state->mutex);
    switch (offset) {
    case TVA_OUT_SETUP_TEXT: {
        tva_textbuf* t = &state->as.text;
        if (width == TC48_OPERAND_WIDTH_6) {
            t->w = value;
            t->h = value;
        } else {
            t->w = (tc48_quarter)((tc48_half)value / TC48_QUARTER_VALUES);
            t->h = (tc48_quarter)((tc48_half)value % TC48_QUARTER_VALUES);
        }

        t->cpos = 0;
        t->cells = malloc(sizeof(tc48_quarter) * t->w * t->h);
        state->mode = TVA_MODE_TEXT;
        break;
    }
    case TVA_OUT_SETUP_GRPH:
        TC48_TODO("graphics mode not implemented yet");
    default:
        switch (state->mode) {
        case TVA_MODE_NONE:
            break;
        case TVA_MODE_TEXT:
            tva_text_mode_out(&state->as.text, offset, width, value);
            break;
        default:
            TC48_UNREACHABLE("invalid TVA mode");
        }
    }
    SDL_UnlockMutex(state->mutex);
}

//////////////////////////////////////// IN ///////////////////////////////////////////
static tc48_word tva_text_mode_in(tva_textbuf* t, tc48_word offset, tc48_width width) {
    switch (offset) {
    case TVA_TEXT_IN_GET_CPOS:
        return t->cpos;
    case TVA_TEXT_IN_GET_CELL:
        if (width == TC48_OPERAND_WIDTH_6) {
            return (tc48_word)(tc48_tryte)((tc48_quarter)t->cells[t->cpos] / TC48_QUARTER_VALUES);
        } else {
            return (tc48_word)t->cells[t->cpos];
        }
    case TVA_TEXT_IN_GET_SIZE:
        if (width == TC48_OPERAND_WIDTH_6) {
            return t->w;
        } else {
            return ((tc48_quarter)t->h * TC48_TRYTE_VALUES) + t->w;
        }
    default:
        // unreachable for now, maybe reachable in the future
        // if graphics mode will need more ports than text mode
        return 0;
    }
}

static tc48_word tva_dev_in(tc48_device* self, tc48_word offset, tc48_width width) {
    tva_dev_state* state = self->context.data;
    SDL_LockMutex(state->mutex);
    tc48_word res = 0;
    switch (offset) {
    case TVA_IN_MAX_SIZE_TEXT:
        if (width == TC48_OPERAND_WIDTH_6) {
            res = TEXT_MAX_SIZE;
        } else {
            res = ((tc48_quarter)TEXT_MAX_SIZE * TC48_TRYTE_VALUES) + TEXT_MAX_SIZE;
        }
        break;
    case TVA_IN_MAX_SIZE_GRPH:
        TC48_TODO("graphics mode not implemented yet");
    default:
        switch (state->mode) {
        case TVA_MODE_NONE:
            break;
        case TVA_MODE_TEXT:
            res = tva_text_mode_in(&state->as.text, offset, width);
            break;
        default:
            TC48_UNREACHABLE("invalid TVA mode");
        }
    }
    SDL_UnlockMutex(state->mutex);
    return res;
}

///////////////////////////////////// DEVICE ///////////////////////////////////////
tc48_device_class tva_device_class = {
    .name = "TVA",
    .ports = TVA_PORTS_COUNT,

    // TVA\0\0\0\0\0 encoded in TSCS
    .type_id = TC48_WORD(
        /*   T   */  /*   V   */  /*   A   */  /*   NUL  */
        0,1,0,0,0,1, 0,1,0,0,1,0, 0,0,2,1,0,0, 0,0,0,0,0,0,
        0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0
        /*  NUL  */  /*  NUL  */  /*  NUL  */  /*  NUL  */
    ),

    .init   = tva_dev_init,
    .deinit = tva_dev_deinit,

    .in     = tva_dev_in,
    .out    = tva_dev_out,
};
