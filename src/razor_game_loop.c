#include <razor.h>
#include <razor/gl.h>

#include "./razor_shader.h"

#include <SDL.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 1080
#define HEIGHT 720

static const char *vertex_shader_source = \
        "#version 330 core\n"
        "layout(location = 0) in vec3 in_Position;\n"
        "out vec4 vertexColor;\n"
        "void main(void) {\n"
        "gl_Position = vec4(in_Position, 1.0);\n"
        "vertexColor = vec4(0.5, 0.5, 0.0, 1.0);\n"
        "}\n";
static const char *fragment_shader_source = \
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 vertexColor;\n"
        "void main(void) {\n"
        "FragColor = vertexColor;\n"
        "}\n";

void rz_RunApplication(const rz_ClientStrategy *strategy)
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

    strategy->init_func(strategy->user_ptr);

    bool running = true;

    float other_vertices[] = {
        0.8f, 0.8f, -0.1f,
        0.8f, -0.8f, -0.1f,
        -0.8f, -0.8f, -0.1f,
        -0.8f, 0.8f, -0.1f
    };

    int other_indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint other_vao;
    glGenVertexArrays(1, &other_vao);
    glBindVertexArray(other_vao);

    GLuint other_positions;
    glGenBuffers(1, &other_positions);
    glBindBuffer(GL_ARRAY_BUFFER, other_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof other_vertices, other_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    GLuint other_ebo;
    glGenBuffers(1, &other_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, other_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof other_indices, other_indices, GL_STATIC_DRAW);

    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f
    };

    int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_DYNAMIC_DRAW);

#define VEC3_SIZE 3

    glVertexAttribPointer(0, VEC3_SIZE, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

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

        strategy->update_func(strategy->user_ptr);
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glEnableVertexAttribArray(0);

        const size_t buffer_length = sizeof vertices / sizeof vertices[0];
        float *buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

        if (!buffer) {
            printf("buffer fucked you over.\n");
            running = false;
        }

        for (int i = 0; i < buffer_length / 3; ++i) {
            buffer[3 * i] += 0.01;
        }

        if (glUnmapBuffer(GL_ARRAY_BUFFER) != GL_TRUE) {
            printf("unmapbuffer fucked you over.\n");
            running = false;
        }
        else {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        }

        glBindVertexArray(other_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, other_ebo);
        glEnableVertexAttribArray(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    strategy->uninit_func(strategy->user_ptr);
}

