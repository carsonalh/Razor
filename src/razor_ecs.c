#include <razor.h>
#include <razor/maths.h>

#include "./razor_render_objects.h"

#include <rigid.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define _RZ_CREATE_COMPONENT(_type) _rz_Component_Create(RZ_COMPONENT_TYPE_ ## _type, NULL)

#define FOR_EACH_LIST(_type, _var, _list) \
        for ( \
                _type *_i = 0, *_var; \
                _var = rg_List_Get(_list, (int)_i), \
                        (int)_i < rg_List_GetSize(_list); \
                ++(int8_t*)_i \
        )

struct rz_Scene {
    rz_Camera *camera;
    rg_List entities;
};

typedef enum {
    RZ_COMPONENT_TYPE_TRANSFORM,
    RZ_COMPONENT_TYPE_RENDER
} rz_ComponentType;

struct rz_Entity {
    rg_List components;
};

typedef void (*rz_ComponentInitFunc)(void *data);
typedef void (*rz_ComponentUpdateFunc)(void *data);
typedef void (*rz_ComponentUninitFunc)(void *data);

struct rz_Component {
    rz_ComponentType type;
    rz_ComponentInitFunc init_func;
    rz_ComponentUpdateFunc update_func;
    rz_ComponentUninitFunc uninit_func;
    void *data;
};

typedef struct rz_RenderComponentData {
    rz_RenderStrategy *render_strategy;
} rz_RenderComponentData;

rz_Scene *rz_Scene_Create(rz_Camera *camera)
{
    rz_Scene *s = malloc(sizeof * s);
    s->camera = camera;
    _rg_List_Init(&s->entities, sizeof(rz_Entity), 0);
    return s;
}

void rz_Scene_Destroy(rz_Scene *scene)
{
    _rg_List_Uninit(&scene->entities);
}

void rz_Scene_Update(rz_Scene *scene)
{
}

void rz_Scene_AddEntity(rz_Scene *scene, const rz_Entity *entity)
{
}

rz_Entity *rz_Entity_Create(void)
{
    rz_Entity *entity = malloc(sizeof * entity);
    _rg_List_Init(&entity->components, sizeof(rz_Component), 0);
    return entity;
}

void rz_Entity_Destroy(rz_Entity *entity)
{
    _rg_List_Uninit(&entity->components);
    free(entity);
}

void rz_Entity_Init(rz_Entity *entity)
{
    FOR_EACH_LIST(rz_Component, c, &entity->components) {
        if (c->init_func) c->init_func(c->data);
    }
}

void rz_Entity_Update(rz_Entity *entity)
{
    FOR_EACH_LIST(rz_Component, c, &entity->components) {
        if (c->update_func) c->update_func(c->data);
    }
}

void rz_Entity_Uninit(rz_Entity *entity)
{
    FOR_EACH_LIST(rz_Component, c, &entity->components) {
        if (c->uninit_func) c->uninit_func(c->data);
    }
}

bool rz_Entity_AddComponent(rz_Entity *entity, const rz_Component *component)
{
    bool has_component = rz_Entity_HasComponent(entity, component);

    if (!has_component)
        rg_List_InsertEnd(&entity->components, component);

    return has_component;
}

bool rz_Entity_RemoveComponent(rz_Entity *entity, const rz_Component *component)
{
    const rz_Component *c;
    const size_t num_components = rg_List_GetSize(&entity->components);

    for (int i = 0; i < num_components; ++i) {
        c = rg_List_Get(&entity->components, i);

        if (c->type == component->type && c->data == component->data) {
            rg_List_Delete(&entity->components, i);
            return true;
        }
    }

    return false;
}

bool rz_Entity_HasComponent(rz_Entity *entity, const rz_Component *component)
{
    const rz_Component *c;
    const size_t num_components = rg_List_GetSize(&entity->components);

    for (int i = 0; i < num_components; ++i) {
        c = rg_List_Get(&entity->components, i);

        if (c->type == component->type && c->data == component->data) {
            return true;
        }
    }

    return false;
}

static inline rz_Component *_rz_Component_Create(rz_ComponentType type, void *data)
{
    rz_Component *c = malloc(sizeof * c);
    c->type = type;
    c->data = data;

    c->init_func = NULL;
    c->update_func = NULL;
    c->uninit_func = NULL;

    return c;
}

rz_Component *rz_TransformComponent_Create(const rz_Transform *transform)
{
    rz_Component *c = _RZ_CREATE_COMPONENT(TRANSFORM);
    rz_Transform *t = malloc(sizeof(rz_Transform));

    memcpy(t, transform, sizeof *t);

    c->data = t;
    return c;
}

void rz_TransformComponent_Destroy(rz_Component *component)
{
    free((rz_Transform *)component->data);
    free(component);
}

static void rz_RenderComponent_UpdateFunc(rz_RenderComponentData *data)
{
}

rz_Component *rz_RenderComponent_Create(void)
{
    rz_Component *c = _RZ_CREATE_COMPONENT(RENDER);
    rz_RenderComponentData *data = malloc(sizeof * data);
    c->data = data;
    c->update_func = rz_RenderComponent_UpdateFunc;
    return c;
}

void rz_RenderComponent_Destroy(rz_Component *component)
{
    free(component->data);
    free(component);
}

