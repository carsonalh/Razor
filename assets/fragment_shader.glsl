#version 330 core

out vec4 FragColor;

in vec2 pass_Position;

void main(void) {
    FragColor = vec4(pass_Position.x, pass_Position.y, 1 - pass_Position.x * pass_Position.y, 1.0);
}

