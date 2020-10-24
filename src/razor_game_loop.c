#include <razor.h>
#include <razor/maths.h>

#include "./razor_ecs.h"
#include "./razor_render_objects.h"
#include "./razor_shader.h"
#include "./razor_utils.h"
#include "./sdl/razor_sdl_input.h"

#include <SDL.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 1080
#define HEIGHT 720

/*
future options:
CLEAN THIS UP OMG IT IS SO BAD ATM
textures
debugging
*/

void rz_RunApplication(const rz_ClientStrategy *client_strategy)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to init SDL.\n");
        return;
    }

    const char *vertex_shader_source = rz_LoadFile("../../../../assets/vertex_shader.glsl");
    const char *fragment_shader_source = rz_LoadFile("../../../../assets/fragment_shader.glsl");

    SDL_Window *window = SDL_CreateWindow("Razor Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!gladLoadGL()) {
        printf("failed to load gl.\n");
        return;
    }

    rz_InputState *input_state = rz_InputState_Create();

    client_strategy->init_func(client_strategy->user_ptr);

    bool running = true;

    rz_Shader *vertex_shader = rz_Shader_Create(RZ_SHADER_TYPE_VERTEX,
        vertex_shader_source);

    rz_Shader *fragment_shader = rz_Shader_Create(RZ_SHADER_TYPE_FRAGMENT,
        fragment_shader_source);

    rz_Camera *main_camera = rz_Camera_Create((float)WIDTH / (float)HEIGHT);
    rz_Renderer *renderer = rz_Renderer_Create(main_camera);

    rz_ShaderProgram *program = rz_ShaderProgram_Create();
    rz_ShaderProgram_AddShader(program, vertex_shader);
    rz_ShaderProgram_AddShader(program, fragment_shader);
    rz_ShaderProgram_Compile(program);

    rz_Transform transform = {
        .position = { -1.0, -1.0 },
        .rotation = 0,
        .scale = { 0.5, 0.5 }
    };

    rz_ComponentStrategy strategy;
    strategy.init_func = rz_QuadComponent_Init;
    strategy.uninit_func = rz_QuadComponent_Uninit;
    strategy.update_func = rz_QuadComponent_Update;

    rz_Component *quad_component = rz_Component_Create(&strategy);

    rz_Entity *quad_entity = rz_Entity_Create();
    rz_Entity_AddComponent(quad_entity, quad_component);

    rz_Entity_Init(quad_entity);

    rz_Scene *scene = rz_Scene_Create(main_camera);

    rz_Quad *other_quad = rz_Quad_Create((vec2) { 0, 0 }, (vec2) { 1, 1 });
    rz_RenderStrategy *other_strategy = rz_Quad_GetRenderStrategy(other_quad, program);

    rz_Quad *third_quad = rz_Quad_Create((vec2) { 0, -2.0f }, (vec2) { 2, 2 });
    rz_RenderStrategy *third_strategy = rz_Quad_GetRenderStrategy(third_quad, program);

    rz_Clear *clear = rz_Clear_Create((vec4) { 0.0f, 0.0f, 0.0f, 1.0f });
    rz_RenderStrategy *clear_strategy = rz_Clear_GetRenderStrategy(clear);

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                rz_InputEvent input_event = {
                    .type = RZ_INPUT_EVENT_KEY_PRESS,
                    .key = rz_SdlKeyToRzKey(event.key.keysym.sym)
                };
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                rz_InputEvent input_event = {
                    .type = RZ_INPUT_EVENT_MOUSE_BUTTON_PRESS,
                    .mouse_button = rz_SdlMouseButtonToRzMouseButton(event.button.button)
                };

                if (input_event.mouse_button == RZ_MOUSE_BUTTON_LEFT) {
                    printf("left mouse clicked.\n");
                }
                else if (input_event.mouse_button == RZ_MOUSE_BUTTON_RIGHT) {
                    printf("right mouse clicked.\n");
                }

                rz_InputState_AddEvent(input_state, &input_event);
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                rz_InputEvent input_event = {
                    .type = RZ_INPUT_EVENT_MOUSE_BUTTON_RELEASE,
                    .mouse_button = rz_SdlMouseButtonToRzMouseButton(event.button.button)
                };

                rz_InputState_AddEvent(input_state, &input_event);
            }
            else if (event.type == SDL_MOUSEMOTION) {
                rz_InputEvent input_event = {
                    .type = RZ_INPUT_EVENT_MOUSE_MOVE,
                    .mouse_move_x = event.motion.xrel / (float)(WIDTH / 2),
                    .mouse_move_y = -event.motion.yrel / (float)(HEIGHT / 2)
                };

                if (rz_InputState_IsMouseButtonDown(input_state, RZ_MOUSE_BUTTON_LEFT)) {
                    printf("{ x: %.2f, y: %.2f }\n", input_event.mouse_move_x, input_event.mouse_move_y);
                    float y_scale;
                    {
                        vec2 scale;
                        rz_Camera_GetScale(main_camera, scale);
                        y_scale = VEC_Y(scale);
                    }
                    vec2 position;
                    rz_Camera_GetPosition(main_camera, position);
                    VEC_X(position) -= input_event.mouse_move_x * y_scale;
                    VEC_Y(position) -= input_event.mouse_move_y * y_scale;
                    rz_Camera_SetPosition(main_camera, position);
                }
            }
            else if (event.type == SDL_MOUSEWHEEL) {
                rz_InputEvent input_event = {
                    .type = RZ_INPUT_EVENT_SCROLL,
                    .mouse_scroll_x = (float)event.wheel.x,
                    .mouse_scroll_y = (float)event.wheel.y
                };

                float factor = 1.0f - input_event.mouse_scroll_y / 5.0f;
                printf("scale: %.2f\n", factor);

                vec2 scale;
                rz_Camera_GetScale(main_camera, scale);
                glm_vec2_scale(scale, factor, scale);
                rz_Camera_SetScale(main_camera, scale);
            }
        }

        rz_Entity_Update(quad_entity);

        client_strategy->update_func(client_strategy->user_ptr);

        rz_Renderer_Render(renderer, clear_strategy);
        rz_Renderer_Render(renderer, other_strategy);
        rz_Renderer_Render(renderer, third_strategy);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }
    
    rz_Entity_Uninit(quad_entity);
    rz_Entity_Destroy(quad_entity);

    rz_Component_Destroy(quad_component);

    rz_Clear_Destroy(clear);
    rz_RenderStrategy_Destroy(clear_strategy);

    rz_Renderer_Destroy(renderer);

    rz_Shader_Destroy(vertex_shader);
    rz_Shader_Destroy(fragment_shader);
    rz_ShaderProgram_Destroy(program);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    client_strategy->uninit_func(client_strategy->user_ptr);
}

