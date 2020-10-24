#ifndef RAZOR_RENDER_OBJECTS_H
#define RAZOR_RENDER_OBJECTS_H

#include <razor.h>
#include <razor/maths.h>

#include "./razor_shader.h"

typedef struct rz_Renderer rz_Renderer;

typedef struct rz_RenderStrategy rz_RenderStrategy;

typedef struct rz_Quad rz_Quad;
typedef struct rz_Clear rz_Clear;

rz_Renderer        *rz_Renderer_Create(rz_Camera *);
void                rz_Renderer_SetCamera(rz_Renderer *, const rz_Camera *);
void                rz_Renderer_Destroy(rz_Renderer *);
void                rz_Renderer_Render(
                            rz_Renderer *, const rz_RenderStrategy *);

void                rz_Camera_GetViewProjectionMatrix(
                            const rz_Camera *, mat4 out);

void                rz_RenderStrategy_Destroy(rz_RenderStrategy *);

rz_Quad            *rz_Quad_Create(vec2, vec2);
void                rz_Quad_Destroy(rz_Quad *);
rz_RenderStrategy  *rz_Quad_GetRenderStrategy(
                            rz_Quad *, const rz_ShaderProgram *);

rz_Clear           *rz_Clear_Create(vec4 color);
void                rz_Clear_Destroy(rz_Clear *);
rz_RenderStrategy  *rz_Clear_GetRenderStrategy(rz_Clear *);

#endif /* RAZOR_RENDER_OBJECTS_H */

