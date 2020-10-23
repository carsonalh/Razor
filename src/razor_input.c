#include <razor.h>

#include <stdlib.h>

struct rz_InputState {
    bool key_states[RZ_INPUT_NUM_KEYS];
    bool mouse_button_states[RZ_INPUT_NUM_MOUSE_BUTTONS];
};

rz_InputState *rz_InputState_Create(void)
{
    rz_InputState *state = malloc(sizeof * state);

    for (int i = 0; i < RZ_INPUT_NUM_KEYS; ++i) {
        state->key_states[i] = false;
    }

    for (int i = 0; i < RZ_INPUT_NUM_MOUSE_BUTTONS; ++i) {
        state->mouse_button_states[i] = false;
    }

    return state;
}

void rz_InputState_Destroy(rz_InputState *state)
{
    free(state);
}

void rz_InputState_AddEvent(rz_InputState *state, const rz_InputEvent *event)
{
    if (event->type == RZ_INPUT_EVENT_KEY_PRESS) {
        state->key_states[event->key] = true;
    }
    else if (event->type == RZ_INPUT_EVENT_KEY_RELEASE) {
        state->key_states[event->key] = false;
    }
    else if (event->type == RZ_INPUT_EVENT_MOUSE_BUTTON_PRESS) {
        state->mouse_button_states[event->mouse_button] = true;
    }
    else if (event->type == RZ_INPUT_EVENT_MOUSE_BUTTON_RELEASE) {
        state->mouse_button_states[event->mouse_button] = false;
    }
}

void rz_InputState_Reset(rz_InputState *state)
{
    for (int i = 0; i < RZ_INPUT_NUM_KEYS; ++i) {
        state->key_states[i] = false;
    }

    for (int i = 0; i < RZ_INPUT_NUM_MOUSE_BUTTONS; ++i) {
        state->mouse_button_states[i] = false;
    }
}

bool rz_InputState_IsKeyDown(rz_InputState *state, const rz_Key key)
{
    return state->key_states[key];
}

bool rz_InputState_IsMouseButtonDown(rz_InputState *state,
        const rz_MouseButton mouse_button)
{
    return state->mouse_button_states[mouse_button];
}

