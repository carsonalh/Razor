#version 330 core

layout (location = 0) in vec2 in_Position;

out vec2 pass_Position;

uniform mat4 u_MvpMatrix;

void main(void) {
    gl_Position = u_MvpMatrix * vec4(in_Position, 0.0, 1.0);
    pass_Position = in_Position;
}

