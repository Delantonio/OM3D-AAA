#version 450

#include "utils.glsl"

layout(binding = 0) uniform Data {
    FrameData frame;
};

layout(std430, binding = 1) buffer ParticleSSBO {
    Particle particles[];
};

layout(binding = 0) uniform sampler2D in_texture;

in vec3 color;
layout(location = 0) out vec4 out_color;

void main() {
    // out_color = particle.color * texture(in_texture, particle.uv);
    // out_color = vec4(color, 1.0);
    out_color = vec4(color, 1.0);
}
