#ifndef RAZOR_H
#define RAZOR_H

#ifdef _WIN32
#   ifdef RZ_BUILD
#       define RZ_EXPORT __declspec(dllexport)
#   else
#       define RZ_EXPORT __declspec(dllimport)
#   endif
#else
#   define RZ_EXPORT
#endif

typedef void (*rz_ClientInitFunc)(void *);
typedef void (*rz_ClientUpdateFunc)(void *);
typedef void (*rz_ClientUninitFunc)(void *);

typedef struct {
    void *user_ptr;
    rz_ClientInitFunc init_func;
    rz_ClientUpdateFunc update_func;
    rz_ClientUninitFunc uninit_func;
} rz_ClientStrategy;

RZ_EXPORT void rz_RunApplication(const rz_ClientStrategy *strategy);

#endif /* RAZOR_H */

