#version 450 core

uniform sampler2D TEXTURE;
uniform vec2 FRAME_SCALE;
uniform vec2 FRAME_OFFSET;

in vec2 v_Texture;

out vec4 Color;

void main() {
    Color = texture(TEXTURE, v_Texture * FRAME_SCALE + FRAME_OFFSET);
}
