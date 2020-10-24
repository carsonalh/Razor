#ifndef RAZOR_MATHS_H
#define RAZOR_MATHS_H

#include <cglm/cglm.h>

#define VEC_X(_vec) ((_vec)[0])
#define VEC_Y(_vec) ((_vec)[1])
#define VEC_Z(_vec) ((_vec)[2])
#define VEC_W(_vec) ((_vec)[3])

#define VEC2_X(_vec) VEC_X(_vec)
#define VEC2_Y(_vec) VEC_Y(_vec)

#define VEC3_X(_vec) VEC_X(_vec)
#define VEC3_Y(_vec) VEC_Y(_vec)
#define VEC3_Z(_vec) VEC_Z(_vec)

#define VEC3_X(_vec) VEC_X(_vec)
#define VEC3_Y(_vec) VEC_Y(_vec)
#define VEC3_Z(_vec) VEC_Z(_vec)
#define VEC3_W(_vec) VEC_W(_vec)

typedef struct rz_Transform rz_Transform;

struct rz_Transform {
    vec2 position;
    float rotation;
    vec2 scale;
};

#endif /* RAZOR_MATHS_H */

