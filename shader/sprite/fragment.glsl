#version 450 core

uniform sampler2D TEXTURE;

in vec2 v_Texture;

out vec4 Color;

void main() {
    Color = texture(TEXTURE, v_Texture);
}
