#include <razor.h>

#include <rigid.h>

#include "./razor_render_objects.h"

struct rz_Scene {
    rz_Camera *camera;
    rg_List entities;
};

struct rz_Entity {
    rz_Scene *scene;
    rz_Transform transform;
    rg_List components;
};

struct rz_Component {
    rz_ComponentTemplate *template;
    rz_Entity *entity;
    void **state;
};

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
    free(scene);
}

rz_Camera *rz_Scene_GetCamera(rz_Scene *scene)
{
    return scene->camera;
}

void rz_Scene_Init(rz_Scene *scene)
{
    size_t num_entities = rg_List_GetSize(&scene->entities);
    for (int i = 0; i < num_entities; ++i) {
        rz_Entity *e = rg_List_Get(&scene->entities, i);
        size_t num_components = rg_List_GetSize(&e->components);

        for (int j = 0; j < num_components; ++j) {
            rz_Component *c = rg_List_Get(&e->components, j);
            c->template->init_func(c, &c->state);
        }
    }
}

void rz_Scene_Update(rz_Scene *scene)
{
    size_t num_entities = rg_List_GetSize(&scene->entities);
    for (int i = 0; i < num_entities; ++i) {
        rz_Entity *e = rg_List_Get(&scene->entities, i);
        size_t num_components = rg_List_GetSize(&e->components);

        for (int j = 0; j < num_components; ++j) {
            rz_Component *c = rg_List_Get(&e->components, j);
            c->template->update_func(c, &c->state);
        }
    }
}

void rz_Scene_Uninit(rz_Scene *scene)
{
    size_t num_entities = rg_List_GetSize(&scene->entities);
    for (int i = 0; i < num_entities; ++i) {
        rz_Entity *e = rg_List_Get(&scene->entities, i);
        size_t num_components = rg_List_GetSize(&e->components);

        for (int j = 0; j < num_components; ++j) {
            rz_Component *c = rg_List_Get(&e->components, j);
            c->template->uninit_func(c, &c->state);
        }
    }
}

rz_Entity *rz_Entity_Create(rz_Scene *scene)
{
    rz_Entity e = {
        .transform = {
            .position = { 0, 0 },
            .rotation = 0.0f,
            .scale = { 1, 1 }
        },
        .scene = scene
    };

    _rg_List_Init(&e.components, sizeof(rz_Component), 0);
    rg_List_InsertEnd(&scene->entities, &e);

    const size_t num_entities = rg_List_GetSize(&scene->entities);
    rz_Entity *in_place = rg_List_Get(&scene->entities, num_entities - 1);

    return in_place;
}

void rz_Entity_Destroy(rz_Entity *entity)
{
    rg_List *scene_entities = &entity->scene->entities;
    rz_Entity *first = rg_List_Get(scene_entities, 0);
    uint index = entity - first;

    _rg_List_Uninit(&entity->components);
    rg_List_Delete(scene_entities, index);
}

bool rz_Entity_HasComponent(rz_Entity *entity, rz_Component *component)
{
    rz_Component *start, *end;
    size_t num_components = rg_List_GetSize(&entity->components);
    start = rg_List_Get(&entity->components, 0);
    end = rg_List_Get(&entity->components, num_components - 1);
    return start <= component && component <= end;
}

rz_Scene *rz_Entity_GetScene(rz_Entity *entity)
{
    return entity->scene;
}

rz_Component *rz_Component_Create(rz_ComponentTemplate *template, rz_Entity *entity)
{
    rz_Component c = {
        .template = template,
        .entity = entity
    };

    rg_List_InsertEnd(&entity->components, &c);
    size_t num_components = rg_List_GetSize(&entity->components);
    rz_Component *in_place = rg_List_Get(&entity->components, num_components - 1);
    return in_place;
}

void rz_Component_Destroy(rz_Component *component)
{
    rg_List *entity_components = &component->entity->components;
    rz_Component *first = rg_List_Get(entity_components, 0);
    uint index = component - first;

    rg_List_Delete(entity_components, index);
}

rz_Entity *rz_Component_GetEntity(rz_Component *component)
{
    return component->entity;
}

rz_Scene *rz_Component_GetScene(rz_Component *component)
{
    return component->entity->scene;
}

