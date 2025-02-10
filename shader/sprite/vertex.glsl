#version 450 core

uniform mat4 MODEL = mat4(1.0);
uniform mat4 VIEW = mat4(1.0);
uniform mat4 PROJECTION = mat4(1.0);

in vec4 Position;
in vec2 Texture;

out vec2 v_Texture;

void main() {
    gl_Position = PROJECTION * VIEW * MODEL * Position;
    v_Texture = Texture;
}
