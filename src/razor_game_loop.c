#include <razor.h>
#include <razor/gl.h>

#include "./razor_render_objects.h"
#include "./razor_shader.h"
#include "./razor_utils.h"

#include <SDL.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 1080
#define HEIGHT 720

/*
future options:
projections/math
user input
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

    rz_ShaderProgram_Bind(program);

    rz_Quad *quad = rz_Quad_Create((vec2) { -1.0f, -1.0f }, (vec2) { 0.5, 0.5 });
    rz_RenderStrategy *quad_strategy = rz_Quad_GetRenderStrategy(quad, program);

    rz_Clear *clear = rz_Clear_Create((vec4) { 0.0f, 0.0f, 1.0f, 1.0f });
    rz_RenderStrategy *clear_strategy = rz_Clear_GetRenderStrategy(clear);

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        vec2 camera_position;
        rz_Camera_GetPosition(main_camera, camera_position);
        camera_position[0] += 0.01f;
        rz_Camera_SetPosition(main_camera, camera_position);

        client_strategy->update_func(client_strategy->user_ptr);

        rz_Renderer_Render(renderer, clear_strategy);
        rz_Renderer_Render(renderer, quad_strategy);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    rz_Quad_Destroy(quad);
    rz_RenderStrategy_Destroy(quad_strategy);

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

