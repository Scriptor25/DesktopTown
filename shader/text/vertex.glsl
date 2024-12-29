#version 330 core

layout (location = 0) in vec4 Position;

out vec2 v_Texture;

uniform mat4 PROJECTION;

void main() {
    gl_Position = PROJECTION * vec4(Position.xy, 0.0, 1.0);
    v_Texture = Position.zw;
}
