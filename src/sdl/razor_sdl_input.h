#ifndef RAZOR_SDL_INPUT_H
#define RAZOR_SDL_INPUT_H

#include <razor.h>

const int RZ_SDL_KEY_MAP[RZ_INPUT_NUM_KEYS];
const int RZ_SDL_MOUSE_BUTTON_MAP[RZ_INPUT_NUM_MOUSE_BUTTONS];

int rz_SdlKeyToRzKey(int);
int rz_SdlMouseButtonToRzMouseButton(int);

#endif /* RAZOR_SDL_INPUT_H */

