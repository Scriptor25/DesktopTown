#version 330 core

in vec2 v_Texture;

layout (location = 0) out vec4 Color;

uniform sampler2D TEXTURE;
uniform vec3 COLOR;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(TEXTURE, v_Texture).r);
    Color = vec4(COLOR, 1.0) * sampled;
}
