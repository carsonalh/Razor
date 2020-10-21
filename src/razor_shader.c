#include <razor/gl.h>

#include "./razor_shader.h"

#include <stdlib.h>
#include <string.h>

static GLenum  shader_type_table[] = {
    [RZ_SHADER_TYPE_VERTEX] = GL_VERTEX_SHADER,
    [RZ_SHADER_TYPE_FRAGMENT] = GL_FRAGMENT_SHADER
};

struct rz_Shader {
    rz_Shader *next;
    GLuint id;
    rz_ShaderType type;
    const char source[];
};

struct rz_ShaderProgram {
    GLuint id;
    rz_Shader *first_shader;
};

rz_Shader *rz_Shader_Create(rz_ShaderType type, const char *source)
{
    const size_t source_length = strlen(source);
    rz_Shader *shader = malloc(sizeof * shader + sizeof(char) * (source_length + 1));

    shader->next = NULL;
    shader->type = type;
    memcpy(shader->source, source, source_length + sizeof(char));

    GLenum gl_type = shader_type_table[type];
    shader->id = glCreateShader(gl_type);
    glShaderSource(shader->id, 1, &source, &source_length);

    /* TODO: GIVE THIS AN ERROR SYSTEM FOR BAD COMPILATIONS */
    glCompileShader(shader->id);

    return shader;
}

void rz_Shader_Destroy(rz_Shader *shader)
{
    /* Destroy all next nodes? No; that would make the function name a liar. */
    glDeleteShader(shader->id);
    free(shader);
}

rz_ShaderProgram *rz_ShaderProgram_Create()
{
    rz_ShaderProgram *shader_program = malloc(sizeof * shader_program);
    shader_program->first_shader = NULL;
    shader_program->id = glCreateProgram();
    return shader_program;
}

void rz_ShaderProgram_Destroy(rz_ShaderProgram *shader_program)
{
    rz_Shader *node = shader_program->first_shader, *next = NULL;

    while (node != NULL) {
        next = node->next;
        rz_Shader_Destroy(node);
        node = next;
    }

    glDeleteProgram(shader_program->id);

    free(shader_program);
}

void rz_ShaderProgram_Bind(rz_ShaderProgram *shader_program)
{
    glUseProgram(shader_program->id);
}

void rz_ShaderProgram_Unbind(rz_ShaderProgram *shader_program)
{
    glUseProgram(0);
}

void rz_ShaderProgram_AddShader(rz_ShaderProgram *shader_program, rz_Shader *shader)
{
    rz_Shader *old_head = shader_program->first_shader;
    shader_program->first_shader = shader;
    shader->next = old_head;
}

void rz_ShaderProgram_Compile(rz_ShaderProgram *program)
{
    rz_Shader *node = program->first_shader;

    while (node != NULL) {
        glAttachShader(program->id, node->id);
        node = node->next;
    }

    /* TODO: GIVE THIS AN ERROR MESSAGE FOR WHEN IT FAILS */
    glLinkProgram(program->id);
}

