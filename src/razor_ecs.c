#include <razor.h>
#include <razor/maths.h>

#include "./razor_ecs.h"
#include "./razor_render_objects.h"

#include <rigid.h>

struct rz_Scene {
    rz_Camera *camera;
    rg_List entities;
};

struct rz_Entity {
    rz_Transform transform;
    rg_List components;
};

struct rz_Component {
    rz_ComponentStrategy *strategy;
    void *state;
};

#define FOR_EACH_LIST(_type, _var, _list) \
        for ( \
                _type *_i = 0, *_var; \
                _var = rg_List_Get(_list, (int)_i), \
                        (int)_i < rg_List_GetSize(_list); \
                ++(int8_t*)_i \
        )

rz_Scene *rz_Scene_Create(rz_Camera *camera)
{
    rz_Scene *scene = malloc(sizeof * scene);
    scene->camera = camera;
    _rg_List_Init(&scene->entities, sizeof(rz_Entity), 0);
    return scene;
}

void rz_Scene_Destroy(rz_Scene *scene)
{
    _rg_List_Uninit(&scene->entities);
    free(scene);
}

rz_Entity *rz_Entity_Create(void)
{
    rz_Entity *e = malloc(sizeof * e);
    _rg_List_Init(&e->components, sizeof(rz_Component), 0);
    return e;
}

void rz_Entity_Destroy(rz_Entity *entity)
{
    _rg_List_Uninit(&entity->components);
    free(entity);
}

void rz_Entity_AddComponent(rz_Entity *entity, rz_Component *component)
{
    rg_List_InsertEnd(&entity->components, component);
}

void rz_Entity_Init(rz_Entity *entity)
{
    FOR_EACH_LIST(rz_Component, c, &entity->components) {
        rz_Component_Init(c, entity);
    }
}

void rz_Entity_Uninit(rz_Entity *entity)
{
    FOR_EACH_LIST(rz_Component, c, &entity->components) {
        rz_Component_Uninit(c, entity);
    }
}

void rz_Entity_Update(rz_Entity *entity)
{
    FOR_EACH_LIST(rz_Component, c, &entity->components) {
        rz_Component_Update(c, entity);
    }
}

rz_Component *rz_Component_Create(rz_ComponentStrategy *strategy)
{
    rz_Component *c = malloc(sizeof * c);
    c->strategy = strategy;
    return c;
}

void rz_Component_Destroy(rz_Component *component)
{
    free(component);
}

void rz_Component_Init(rz_Component *component, rz_Entity *entity)
{
    if (component->strategy->init_func) {
        component->strategy->init_func(entity, &component->state);
    }
}

void rz_Component_Uninit(rz_Component *component, rz_Entity *entity)
{
    if (component->strategy->uninit_func) {
        component->strategy->uninit_func(entity, &component->state);
    }
}

void rz_Component_Update(rz_Component *component, rz_Entity *entity)
{
    if (component->strategy->update_func) {
        component->strategy->update_func(entity, &component->state);
    }
}

typedef struct {
    rz_Quad *quad;
    rz_RenderStrategy *strategy;
} QuadState;

void rz_QuadComponent_Init(rz_Entity *entity, QuadState **state)
{
    *state = malloc(sizeof * *state);
    (*state)->quad = rz_Quad_Create(
            entity->transform.position, entity->transform.scale);
    (*state)->strategy = rz_Quad_GetRenderStrategy((*state)->quad, /* refer to rz_QuadComponent_Update() */);
}

void rz_QuadComponent_Uninit(rz_Entity *entity, QuadState **state)
{
    printf("quad uninit here.\n");
}

void rz_QuadComponent_Update(rz_Entity *entity, QuadState **state) {
    /*
PROBLEM: WE NEED ACCESS TO THE RENDERER HERE, AND DON'T EVEN HAVE ACCESS TO THE
SCENE; (OR THE CAMERA) THERE ARE A FEW OPTIONS WE CAN GO THROUGH TO SOLVE THIS
PROBLEM:
1. MAKE THE RENDERER A SINGLETON
2. GIVE EACH ENTITY A POINTER TO ITS RESPECTIVE SCENE, AND GIVE THE SCENE A
POINTER TO THE RENDERER
3. RE-DESIGN THE RENDERER SO THAT IT IS NOT STATE-BASED AT ALL (ALTHOUGH YOU'LL
HAVE TO CONVINCE THE SYSTEM DESIGNER THAT THE RENDERE WON'T HAVE TO HAVE ANY
CPU STATE IN ITS LIFECYCLE THROUGHOUT THE ENTIRE DEVELOPMENT OF THE ENGINE ->
ACTUALLY THIS SEEMS PLAUSIBLE, ATM THE ONLY STATE THE RENDERER HAS IS THE
CAMERA, BUT IF WE PASS THAT, ALONG WITH THE RENDER TARGET, WE COULD TURN THE
RENDER STATE SYSTEM INTO ONE FUNCTION. THIS COULD EVEN WORK FOR BATCH SYSTEMS,
WHERE THE BATCH IS PREPARED AS ONE TARGET USING SOME OTHER SYSTEM, THAT CAN
THEN BE RENDERED AS IF IT WERE ONE OBJECT BY THE RENDERER. */
}

