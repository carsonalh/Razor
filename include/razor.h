#ifndef RAZOR_H
#define RAZOR_H

#include "./razor/maths.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#   ifdef RZ_BUILD
#       define RZ_EXPORT __declspec(dllexport)
#   else
#       define RZ_EXPORT __declspec(dllimport)
#   endif
#else
#   define RZ_EXPORT
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ull;

typedef long long ll;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef void (*rz_ClientInitFunc)(void *);
typedef void (*rz_ClientUpdateFunc)(void *);
typedef void (*rz_ClientUninitFunc)(void *);

typedef struct {
    void *user_ptr;
    rz_ClientInitFunc init_func;
    rz_ClientUpdateFunc update_func;
    rz_ClientUninitFunc uninit_func;
} rz_ClientStrategy;

RZ_EXPORT void rz_RunApplication(const rz_ClientStrategy *strategy);

#define RZ_INPUT_NUM_KEYS 256
#define RZ_INPUT_NUM_MOUSE_BUTTONS 16

typedef struct rz_InputState rz_InputState;
typedef struct rz_InputEvent rz_InputEvent;

typedef enum {
    RZ_KEY_A,
    RZ_KEY_B,
    RZ_KEY_C,
    RZ_KEY_D,
    RZ_KEY_E,
    RZ_KEY_F,
    RZ_KEY_G,
    RZ_KEY_H,
    RZ_KEY_I,
    RZ_KEY_J,
    RZ_KEY_K,
    RZ_KEY_L,
    RZ_KEY_M,
    RZ_KEY_N,
    RZ_KEY_O,
    RZ_KEY_P,
    RZ_KEY_Q,
    RZ_KEY_R,
    RZ_KEY_S,
    RZ_KEY_T,
    RZ_KEY_U,
    RZ_KEY_V,
    RZ_KEY_W,
    RZ_KEY_X,
    RZ_KEY_Y,
    RZ_KEY_Z,
    RZ_KEY_UP,
    RZ_KEY_DOWN,
    RZ_KEY_LEFT,
    RZ_KEY_RIGHT,
    RZ_KEY_RETURN,
    RZ_KEY_BACKSPACE,
    RZ_KEY_ALT,
    RZ_KEY_CTRL,
    RZ_KEY_ESC
} rz_Key;

typedef enum {
    RZ_MOUSE_BUTTON_LEFT,
    RZ_MOUSE_BUTTON_RIGHT,
    RZ_MOUSE_BUTTON_MIDDLE,
    RZ_MOUSE_BUTTON_PREVIOUS,
    RZ_MOUSE_BUTTON_NEXT
} rz_MouseButton;

typedef enum {
    RZ_INPUT_EVENT_KEY_PRESS,
    RZ_INPUT_EVENT_KEY_RELEASE,
    RZ_INPUT_EVENT_MOUSE_BUTTON_PRESS,
    RZ_INPUT_EVENT_MOUSE_BUTTON_RELEASE,
    RZ_INPUT_EVENT_MOUSE_MOVE,
    RZ_INPUT_EVENT_SCROLL,
} rz_InputEventType;

struct rz_InputEvent {
    rz_InputEventType type;
    union {
        rz_Key key;
        rz_MouseButton mouse_button;
        struct {
            float mouse_move_x, mouse_move_y;
        };
        struct {
            float mouse_scroll_x, mouse_scroll_y;
        };
    };
};

RZ_EXPORT rz_InputState    *rz_InputState_Create(void);
RZ_EXPORT void              rz_InputState_Destroy(rz_InputState *);
RZ_EXPORT void              rz_InputState_AddEvent(
                                    rz_InputState *, const rz_InputEvent *);
RZ_EXPORT void              rz_InputState_Reset(rz_InputState *);
RZ_EXPORT bool              rz_InputState_IsKeyDown(rz_InputState *,
                                    const rz_Key);
RZ_EXPORT bool              rz_InputState_IsMouseButtonDown(rz_InputState *,
                                    const rz_MouseButton);

typedef struct rz_Camera rz_Camera;

RZ_EXPORT rz_Camera    *rz_Camera_Create(float aspect_ratio);
RZ_EXPORT void          rz_Camera_GetPosition(const rz_Camera *, vec2);
RZ_EXPORT void          rz_Camera_GetScale(const rz_Camera *, vec2);
RZ_EXPORT float         rz_Camera_GetRotation(const rz_Camera *);
RZ_EXPORT float         rz_Camera_GetAspectRatio(const rz_Camera *);
RZ_EXPORT void          rz_Camera_SetPosition(rz_Camera *, vec2);
RZ_EXPORT void          rz_Camera_SetScale(rz_Camera *, vec2);
RZ_EXPORT void          rz_Camera_SetRotation(rz_Camera *, float);
RZ_EXPORT void          rz_Camera_SetAspectRatio(rz_Camera *, float);

typedef struct rz_Scene rz_Scene;
typedef struct rz_Entity rz_Entity;
typedef struct rz_Component rz_Component;

typedef void (*rz_ComponentInitFunc)(rz_Component *, void **state);
typedef void (*rz_ComponentUpdateFunc)(rz_Component *, void **state);
typedef void (*rz_ComponentUninitFunc)(rz_Component *, void **state);

typedef struct {
    rz_ComponentInitFunc init_func;
    rz_ComponentUpdateFunc update_func;
    rz_ComponentUninitFunc uninit_func;
} rz_ComponentTemplate;

RZ_EXPORT rz_Scene     *rz_Scene_Create(rz_Camera *);
RZ_EXPORT void          rz_Scene_Destroy(rz_Scene *);
RZ_EXPORT rz_Camera    *rz_Scene_GetCamera(rz_Scene *);
RZ_EXPORT void          rz_Scene_Init(rz_Scene *);
RZ_EXPORT void          rz_Scene_Update(rz_Scene *);
RZ_EXPORT void          rz_Scene_Uninit(rz_Scene *);

RZ_EXPORT rz_Entity    *rz_Entity_Create(rz_Scene *);
RZ_EXPORT void          rz_Entity_Destroy(rz_Entity *);
RZ_EXPORT bool          rz_Entity_HasComponent(rz_Entity *, rz_Component *);
RZ_EXPORT rz_Scene     *rz_Entity_GetScene(rz_Entity *);

RZ_EXPORT rz_Component *rz_Component_Create(rz_ComponentTemplate *, rz_Entity *);
RZ_EXPORT void          rz_Component_Destroy(rz_Component *);
RZ_EXPORT rz_Entity    *rz_Component_GetEntity(rz_Component *);
RZ_EXPORT rz_Scene     *rz_Component_GetScene(rz_Component *);

#endif /* RAZOR_H */

