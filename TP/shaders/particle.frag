#version 450

#include "utils.glsl"

layout(binding = 0) uniform sampler2D in_texture;

in vec4 color;
in vec3 normal;
in vec2 uv;

layout(location = 0) out vec4 g_color;
layout(location = 1) out vec3 g_normal;

void main() {
    if (color.a <= 0.001f) {
        discard;
    }
    g_color = color;
    g_normal = normal;
    // out_color = vec4(color, 1.0) * texture(in_texture, uv);
}
