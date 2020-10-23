#include "./razor_sdl_input.h"

#include <SDL.h>

const int RZ_SDL_KEY_MAP[RZ_INPUT_NUM_KEYS] = {
    [SDLK_a] = RZ_KEY_A,
    [SDLK_b] = RZ_KEY_B,
    [SDLK_c] = RZ_KEY_C,
    [SDLK_d] = RZ_KEY_D,
    [SDLK_e] = RZ_KEY_E,
    [SDLK_f] = RZ_KEY_F,
    [SDLK_g] = RZ_KEY_G,
    [SDLK_h] = RZ_KEY_H,
    [SDLK_i] = RZ_KEY_I,
    [SDLK_j] = RZ_KEY_J,
    [SDLK_k] = RZ_KEY_K,
    [SDLK_l] = RZ_KEY_L,
    [SDLK_m] = RZ_KEY_M,
    [SDLK_n] = RZ_KEY_N,
    [SDLK_o] = RZ_KEY_O,
    [SDLK_p] = RZ_KEY_P,
    [SDLK_q] = RZ_KEY_Q,
    [SDLK_r] = RZ_KEY_R,
    [SDLK_s] = RZ_KEY_S,
    [SDLK_t] = RZ_KEY_T,
    [SDLK_u] = RZ_KEY_U,
    [SDLK_v] = RZ_KEY_V,
    [SDLK_w] = RZ_KEY_W,
    [SDLK_x] = RZ_KEY_X,
    [SDLK_y] = RZ_KEY_Y,
    [SDLK_z] = RZ_KEY_Z,
    [SDLK_RETURN] = RZ_KEY_RETURN,
    [SDLK_BACKSPACE] = RZ_KEY_BACKSPACE,
    /* Not really sure how else you're meant to solve this, but for now, it
     * seems that this should work. */
    [SDLK_UP & ~(1<<30)] = RZ_KEY_UP,
    [SDLK_DOWN & ~(1<<30)] = RZ_KEY_DOWN,
    [SDLK_LEFT & ~(1<<30)] = RZ_KEY_LEFT,
    [SDLK_RIGHT & ~(1<<30)] = RZ_KEY_RIGHT,
    [SDLK_LCTRL & ~(1<<30)] = RZ_KEY_CTRL,
    [SDLK_RCTRL & ~(1<<30)] = RZ_KEY_CTRL,
    [SDLK_LALT & ~(1<<30)] = RZ_KEY_ALT,
    [SDLK_RALT & ~(1<<30)] = RZ_KEY_ALT,
};

const int RZ_SDL_MOUSE_BUTTON_MAP[RZ_INPUT_NUM_MOUSE_BUTTONS] = {
    [SDL_BUTTON_LEFT] = RZ_MOUSE_BUTTON_LEFT,
    [SDL_BUTTON_MIDDLE] = RZ_MOUSE_BUTTON_MIDDLE,
    [SDL_BUTTON_RIGHT] = RZ_MOUSE_BUTTON_RIGHT,
};

int rz_SdlKeyToRzKey(int sdl_key)
{
    if (sdl_key & (1 << 30)) {
        sdl_key &= ~(1 << 30);
    }

    return RZ_SDL_KEY_MAP[sdl_key];
}

int rz_SdlMouseButtonToRzMouseButton(int sdl_mouse_button)
{
    return RZ_SDL_MOUSE_BUTTON_MAP[sdl_mouse_button];
}

