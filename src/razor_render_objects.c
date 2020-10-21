#include "./razor_render_objects.h"

#include <razor/gl.h>

#include <string.h>

#define RZ_QUAD_POSITIONS_SLOT 0

struct rz_Renderer {
    int _;
};

struct rz_Quad {
    vec2 size;
    GLuint vao_id, positions_vbo_id, ebo_id;
    int positions_slot;
};

struct rz_Clear {
    vec4 color;
};

typedef void (*rz_RenderFunc)(void *instance);

struct rz_RenderStrategy {
    void *instance;
    rz_RenderFunc render_func;
};

static void rz_Quad_RenderFunc(rz_Quad *quad)
{
    glBindVertexArray(quad->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo_id);

    glEnableVertexAttribArray(quad->positions_slot);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(quad->positions_slot);
}

static void rz_Clear_RenderFunc(rz_Clear *clear)
{
    vec4 color;
    memcpy(color, clear->color, sizeof(vec4));

    float r = color[0], g = color[1], b = color[2], a = color[3];
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

rz_Renderer *rz_Renderer_Create(void)
{
    return malloc(sizeof(rz_Renderer));
}

void rz_Renderer_Destroy(rz_Renderer *renderer)
{
    free(renderer);
}

void rz_Renderer_Render(rz_Renderer *renderer, rz_RenderStrategy *strategy)
{
    strategy->render_func(strategy->instance);
}

rz_Quad *rz_Quad_Create(vec2 size)
{
    rz_Quad *quad = malloc(sizeof * quad);
    memcpy(&quad->size, size, sizeof size);

    quad->positions_slot = RZ_QUAD_POSITIONS_SLOT;

    glGenVertexArrays(1, &quad->vao_id);
    glGenBuffers(1, &quad->positions_vbo_id);
    glGenBuffers(1, &quad->ebo_id);

    glBindVertexArray(quad->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, quad->positions_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo_id);

    float x = quad->size[0], y = quad->size[1];

    float vertices[2 * 4] = {
        0, 0,
        x, 0,
        x, y,
        0, y,
    };

    unsigned indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(quad->positions_slot, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return quad;
}

void rz_Quad_Destroy(rz_Quad *quad)
{
    free(quad);
}

rz_Clear *rz_Clear_Create(vec4 color)
{
    rz_Clear *clear = malloc(sizeof * clear);
    memcpy(&clear->color, color, sizeof (vec4));
    return clear;
}

void rz_Clear_Destroy(rz_Clear *clear)
{
    free(clear);
}

rz_RenderStrategy *rz_Clear_GetRenderStrategy(rz_Clear *clear)
{
    rz_RenderStrategy *strategy = malloc(sizeof * strategy);
    strategy->instance = clear;
    strategy->render_func = rz_Clear_RenderFunc;
    return strategy;
}

rz_RenderStrategy *rz_Quad_GetRenderStrategy(rz_Quad *quad)
{
    rz_RenderStrategy *strategy = malloc(sizeof * strategy);
    strategy->instance = quad;
    strategy->render_func = rz_Quad_RenderFunc;
}

void rz_RenderStrategy_Destroy(rz_RenderStrategy *strategy)
{
    free(strategy);
}

