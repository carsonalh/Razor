#ifndef RAZOR_SHADER_H
#define RAZOR_SHADER_H

#include <razor/gl.h>

typedef enum {
    RZ_SHADER_TYPE_VERTEX,
    RZ_SHADER_TYPE_FRAGMENT,
} rz_ShaderType;

typedef struct rz_Shader rz_Shader;
typedef struct rz_ShaderProgram {
    GLuint id;
    rz_Shader *first_shader;
} rz_ShaderProgram;

rz_Shader  *rz_Shader_Create(rz_ShaderType type, const char *source);
void        rz_Shader_Destroy(rz_Shader *shader);

rz_ShaderProgram   *rz_ShaderProgram_Create();
void                rz_ShaderProgram_Destroy(rz_ShaderProgram *shader_program);
void                rz_ShaderProgram_Bind(rz_ShaderProgram *shader_program);
void                rz_ShaderProgram_Unbind(rz_ShaderProgram *shader_program);
void                rz_ShaderProgram_AddShader(
                            rz_ShaderProgram *shader_program,
                            rz_Shader *shader);
void                rz_ShaderProgram_Compile(rz_ShaderProgram *program);

#endif /* RAZOR_SHADER_H */
