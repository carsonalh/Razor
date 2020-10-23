#include "./razor_render_objects.h"
#include "./razor_shader.h"

#include <razor/gl.h>
#include <razor/maths.h>

#include <string.h>

#define RZ_QUAD_POSITIONS_SLOT 0

struct rz_Renderer {
    const rz_Camera *camera;
};

struct rz_Camera {
    vec2 position, scale;
    float rotation_deg;
    float aspect_ratio;
};

struct rz_Quad {
    vec2 position, size;
    GLuint vao_id, vertices_vbo_id, ebo_id;
    GLuint mvp_matrix_location;
    int vertices_slot;
};

struct rz_Clear {
    vec4 color;
};

typedef void (*rz_RenderFunc)(void *instance, mat4 camera_matrix);
typedef void (*rz_ShaderBindFunc)(void *instance, rz_ShaderProgram *program);
typedef void (*rz_ShaderUnbindFunc)(void *instance, rz_ShaderProgram *program);

struct rz_RenderStrategy {
    void *instance;
    rz_ShaderProgram *shader_program;
    rz_RenderFunc render_func;
    rz_ShaderBindFunc shader_bind_func;
    rz_ShaderUnbindFunc shader_unbind_func;
};

static void rz_Quad_ShaderBindFunc(rz_Quad *quad, rz_ShaderProgram *program)
{
    quad->mvp_matrix_location = glGetUniformLocation(program->id, "u_MvpMatrix");
}

static void rz_Quad_ShaderUnbindFunc(rz_Quad *quad, rz_ShaderProgram *program)
{
}

static void rz_Quad_RenderFunc(rz_Quad *quad, mat4 camera_matrix)
{
    mat4 mvp_matrix;
    mat4 model_matrix;

    glm_mat4_identity(model_matrix);
    VEC_X(model_matrix[3]) = VEC_X(quad->position);
    VEC_Y(model_matrix[3]) = VEC_Y(quad->position);
    VEC_X(model_matrix[0]) = VEC_X(quad->size);
    VEC_Y(model_matrix[1]) = VEC_Y(quad->size);

    glm_mat4_mul(camera_matrix, model_matrix, mvp_matrix);

    glUniformMatrix4fv(quad->mvp_matrix_location, 1, GL_FALSE, mvp_matrix);
    glBindVertexArray(quad->vao_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo_id);

    glEnableVertexAttribArray(quad->vertices_slot);

#define NUM_TRIANGLES_IN_QUAD 2
    glDrawElements(GL_TRIANGLES, 3 * NUM_TRIANGLES_IN_QUAD, GL_UNSIGNED_INT, NULL);
#undef NUM_TRIANGLES_IN_QUAD

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(quad->vertices_slot);
}

static void rz_Clear_ShaderBindFunc(rz_Clear *clear, rz_ShaderProgram *_)
{
}

static void rz_Clear_ShaderUnbindFunc(rz_Clear *clear, rz_ShaderProgram *_)
{
}

static void rz_Clear_RenderFunc(rz_Clear *clear, mat4 _)
{
    vec4 color;
    memcpy(color, clear->color, sizeof(vec4));

    float r = VEC_X(color), g = VEC_Y(color), b = VEC_Z(color), a = VEC_W(color);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

rz_Renderer *rz_Renderer_Create(const rz_Camera *camera)
{
    rz_Renderer *renderer = malloc(sizeof(rz_Renderer));
    renderer->camera = camera;
    return renderer;
}

void rz_Renderer_SetCamera(rz_Renderer *renderer, const rz_Camera *camera)
{
    renderer->camera = camera;
}

void rz_Renderer_Destroy(rz_Renderer *renderer)
{
    free(renderer);
}

void rz_Renderer_Render(rz_Renderer *renderer, const rz_RenderStrategy *strategy)
{
    mat4 camera_matrix;
    rz_Camera_GetViewProjectionMatrix(renderer->camera, camera_matrix);

    strategy->shader_bind_func(strategy->instance, strategy->shader_program);
    strategy->render_func(strategy->instance, camera_matrix);
    strategy->shader_unbind_func(strategy->instance, strategy->shader_program);
}

rz_Camera *rz_Camera_Create(float aspect_ratio)
{
    rz_Camera *camera = malloc(sizeof * camera);

    memcpy(&camera->position, (const vec2) { 0.0f, 0.0f }, sizeof(const vec2));
    memcpy(&camera->scale, (const vec2) { 1.0f, 1.0f }, sizeof(const vec2));
    camera->rotation_deg = 0.0f;
    camera->aspect_ratio = aspect_ratio;

    return camera;
}

void rz_Camera_GetPosition(const rz_Camera *camera, vec2 position)
{
    memcpy(position, camera->position, sizeof position);
}

void rz_Camera_GetScale(const rz_Camera *camera, vec2 scale)
{
    memcpy(scale, camera->scale, sizeof scale);
}

float rz_Camera_GetRotation(const rz_Camera *camera)
{
    return camera->rotation_deg;
}

float rz_Camera_GetAspectRatio(const rz_Camera *camera)
{
    return camera->aspect_ratio;
}

void rz_Camera_SetPosition(rz_Camera *camera, vec2 position)
{
    memcpy(&camera->position, position, sizeof position);
}

void rz_Camera_SetScale(rz_Camera *camera, vec2 scale)
{
    memcpy(&camera->scale, scale, sizeof scale);
}

void rz_Camera_SetRotation(rz_Camera *camera, float angle_deg)
{
    camera->rotation_deg = angle_deg;
}

void rz_Camera_SetAspectRatio(rz_Camera *camera, float aspect_ratio)
{
    camera->aspect_ratio = aspect_ratio;
}

void rz_Camera_GetViewProjectionMatrix(const rz_Camera *camera, mat4 out)
{
    mat4 transform_matrix;

    glm_mat4_identity(transform_matrix);
    VEC_X(transform_matrix[0]) = VEC_X(camera->scale);
    VEC_Y(transform_matrix[1]) = VEC_Y(camera->scale);
    VEC_X(transform_matrix[3]) = VEC_X(camera->position);
    VEC_Y(transform_matrix[3]) = VEC_Y(camera->position);

    mat4 view_matrix;
    glm_mat4_inv(transform_matrix, view_matrix);

    mat4 projection_matrix;
    glm_ortho_default(camera->aspect_ratio, projection_matrix);

    mat4 view_projection_matrix;
    glm_mat4_mul(projection_matrix, view_matrix, view_projection_matrix);

    glm_mat4_copy(view_projection_matrix, out);
}

rz_Quad *rz_Quad_Create(vec2 position, vec2 size)
{
    rz_Quad *quad = malloc(sizeof * quad);
    memcpy(&quad->size, size, sizeof size);
    memcpy(&quad->position, position, sizeof position);

    quad->vertices_slot = RZ_QUAD_POSITIONS_SLOT;

    glGenVertexArrays(1, &quad->vao_id);
    glGenBuffers(1, &quad->vertices_vbo_id);
    glGenBuffers(1, &quad->ebo_id);

    glBindVertexArray(quad->vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, quad->vertices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad->ebo_id);

    float x = VEC_X(quad->size), y = VEC_Y(quad->size);

    float vertices[2 * 4] = {
        0, 0,
        1, 0,
        1, 1,
        0, 1,
    };

    unsigned indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(quad->vertices_slot, 2, GL_FLOAT, GL_FALSE, 0, NULL);

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
    strategy->shader_program = NULL;
    strategy->render_func = rz_Clear_RenderFunc;
    strategy->shader_bind_func = rz_Clear_ShaderBindFunc;
    strategy->shader_unbind_func = rz_Clear_ShaderUnbindFunc;
    return strategy;
}

rz_RenderStrategy *rz_Quad_GetRenderStrategy(rz_Quad *quad, const rz_ShaderProgram *program)
{
    rz_RenderStrategy *strategy = malloc(sizeof * strategy);
    strategy->instance = quad;
    strategy->shader_program = program;
    strategy->render_func = rz_Quad_RenderFunc;
    strategy->shader_bind_func = rz_Quad_ShaderBindFunc;
    strategy->shader_unbind_func = rz_Quad_ShaderUnbindFunc;
}

void rz_RenderStrategy_Destroy(rz_RenderStrategy *strategy)
{
    free(strategy);
}

