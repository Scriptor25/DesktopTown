#version 450 core

in vec2 v_Texture;

out vec4 Color;

uniform sampler2D TEXTURE;
uniform vec3 COLOR = vec3(1.0);

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(TEXTURE, v_Texture).r);
    Color = vec4(COLOR, 1.0) * sampled;
}
