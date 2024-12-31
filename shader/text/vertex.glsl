#version 330 core

in vec4 Position;
in vec2 Texture;

out vec2 v_Texture;

uniform mat4 PROJECTION;

void main() {
    gl_Position = PROJECTION * Position;
    v_Texture = Texture;
}
