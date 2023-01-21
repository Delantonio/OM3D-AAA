#version 450

#include "utils.glsl"

layout(binding = 0) uniform sampler2D in_texture;

in vec3 color;
in vec2 uv;

layout(location = 0) out vec4 out_color;

void main() {
    out_color = vec4(color, 1.0);
    // out_color = vec4(color, 1.0) * texture(in_texture, uv);
}
