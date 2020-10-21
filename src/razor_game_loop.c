#include <razor.h>
#include <razor/gl.h>

#include "./razor_shader.h"
#include "./razor_render_objects.h"

#include <SDL.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 1080
#define HEIGHT 720

static const char *vertex_shader_source = \
        "#version 330 core\n"
        "layout(location = 0) in vec2 in_Position;\n"
        "out vec4 vertexColor;\n"
        "void main(void) {\n"
        "gl_Position = vec4(in_Position, 0.0, 1.0);\n"
        "vertexColor = vec4(0.5, 0.5, 0.0, 1.0);\n"
        "}\n";
static const char *fragment_shader_source = \
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 vertexColor;\n"
        "void main(void) {\n"
        "FragColor = vertexColor;\n"
        "}\n";

void rz_RunApplication(const rz_ClientStrategy *client_strategy)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to init SDL.\n");
        return;
    }

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

    rz_Renderer *renderer = rz_Renderer_Create();

    rz_Quad *quad = rz_Quad_Create((vec2) { 0.5, 0.5 });
    rz_RenderStrategy *quad_strategy = rz_Quad_GetRenderStrategy(quad);

    rz_Clear *clear = rz_Clear_Create((vec4) { 0.0f, 0.0f, 1.0f, 1.0f });
    rz_RenderStrategy *clear_strategy = rz_Clear_GetRenderStrategy(clear);

    rz_Shader *vertex_shader = rz_Shader_Create(RZ_SHADER_TYPE_VERTEX,
            vertex_shader_source);

    rz_Shader *fragment_shader = rz_Shader_Create(RZ_SHADER_TYPE_FRAGMENT,
            fragment_shader_source);

    rz_ShaderProgram *program = rz_ShaderProgram_Create();
    rz_ShaderProgram_AddShader(program, vertex_shader);
    rz_ShaderProgram_AddShader(program, fragment_shader);
    rz_ShaderProgram_Compile(program);

    rz_ShaderProgram_Bind(program);

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

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

