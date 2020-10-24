#ifndef RAZOR_ECS_H
#define RAZOR_ECS_H

#include <razor.h>

void rz_Component_Init(rz_Component *, rz_Entity *);
void rz_Component_Uninit(rz_Component *, rz_Entity *);
void rz_Component_Update(rz_Component *, rz_Entity *);

#endif /* RAZOR_ECS_H */

