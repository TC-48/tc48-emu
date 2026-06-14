#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL.h>

#include "internal.h"
#include <stdio.h>

#define INCBIN_PREFIX
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>

INCBIN(default_font, "assets/tva-default-font.png");

void tva_global_init() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}
void tva_global_deinit() {
    SDL_Quit();
}

static int err(const char* msg) {
    fprintf(stderr, "[TVA] %s: %s\n", msg, SDL_GetError());
    // SDL_WaitThread return -1 if the thread has been detached or isn't valid, so we'll use -2
    return -2;
}

static void tva_get_color(tc48_tryte triplet, uint8_t* r, uint8_t* g, uint8_t* b) {
    int rv = (triplet / 9) % 3;
    int gv = (triplet / 3) % 3;
    int bv = triplet % 3;

    *r = (uint8_t)(rv * 255 / 2);
    *g = (uint8_t)(gv * 255 / 2);
    *b = (uint8_t)(bv * 255 / 2);
}

static void tva_render_text(SDL_Renderer* renderer, SDL_Texture* font_tex, tva_textbuf* t) {
    if (t->cells == NULL || font_tex == NULL) return;

    for (int y = 0; y < (int)t->h; y++) {
        for (int x = 0; x < (int)t->w; x++) {
            tc48_quarter cell = t->cells[y * (int)t->w + x];
            tc48_tryte color_tryte = (tc48_tryte)(cell / TC48_TRYTE_VALUES);
            tc48_tryte ch          = (tc48_tryte)(cell % TC48_TRYTE_VALUES);

            tc48_tryte bg_triplet = (color_tryte / 27) % 27;
            tc48_tryte fg_triplet = color_tryte % 27;

            uint8_t br, bg, bb;
            tva_get_color(bg_triplet, &br, &bg, &bb);

            uint8_t fr, fg, fb;
            tva_get_color(fg_triplet, &fr, &fg, &fb);

            SDL_FRect dst = { (float)x * 28, (float)y * 28, 28.0f, 28.0f };

            SDL_SetRenderDrawColor(renderer, br, bg, bb, 255);
            SDL_RenderFillRect(renderer, &dst);

            int row = ch / 27;
            int col = ch % 27;
            SDL_FRect src = { (float)col * 28, (float)row * 28, 28.0f, 28.0f };

            SDL_SetTextureColorMod(font_tex, fr, fg, fb);
            SDL_RenderTexture(renderer, font_tex, &src, &dst);
        }
    }
}

int tva_loop(void* ctx) {
    tva_dev_state* state = ctx;
    SDL_Window* window = SDL_CreateWindow(
        "TC-48 Video Adapter",
        800, 600,
       /*flags=*/0
    );

    if (window == NULL) {
        return err("window creation failed");
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        return err("renderer creation failed");
    }

    SDL_IOStream* io = SDL_IOFromConstMem(default_font_data, default_font_size);
    SDL_Texture* font_tex = IMG_LoadTexture_IO(renderer, io, true);
    if (font_tex == NULL) {
        fprintf(stderr, "[TVA] failed to load font texture: %s\n", SDL_GetError());
    }

    SDL_Event event;
    while (SDL_GetAtomicInt(&state->running)) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SDL_SetAtomicInt(&state->running, 0);
                break;
            }
        }

        SDL_LockMutex(state->mutex);
        if (state->mode == TVA_MODE_TEXT) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            tva_render_text(renderer, font_tex, &state->as.text);
        } else {
            SDL_SetRenderDrawColor(renderer, 33, 150, 243, 255);
            SDL_RenderClear(renderer);
        }
        SDL_UnlockMutex(state->mutex);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    if (font_tex) SDL_DestroyTexture(font_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}

bool tva_start_in_bg(tva_dev_state* state) {
    SDL_SetAtomicInt(&state->running, 1);
    state->thread = SDL_CreateThread(tva_loop, "TVA", state);
    return state->thread != NULL;
}

bool tva_stop_bg(tva_dev_state* state) {
    if (state->thread == NULL) return true;
    SDL_SetAtomicInt(&state->running, 0);
    int r;
    SDL_WaitThread(state->thread, &r);
    state->thread = NULL;
    return r >= 0;
}
