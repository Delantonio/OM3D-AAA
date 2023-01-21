#version 450

#include "utils.glsl"

layout(location = 0) in vec3 in_pos;
// layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
// layout(location = 3) in vec4 in_tangent_bitangent_sign;
// layout(location = 4) in vec3 in_color;

// layout(location = 0) out vec3 out_normal;
// layout(location = 1) out vec2 out_uv;
// layout(location = 2) out vec3 out_color;
// layout(location = 3) out vec3 out_position;
// layout(location = 4) out vec3 out_tangent;
// layout(location = 5) out vec3 out_bitangent;

layout(binding = 0) uniform Data {
    FrameData frame;
};

layout(binding = 1) buffer ParticleSSBO {
    Particle particles[];
};

out vec3 color;
out vec2 uv;

void main() {
    
    mat4 view = frame.camera.view;
    mat4 proj = frame.camera.proj;

    vec4 modelSpace = view * vec4(particles[gl_InstanceID].center, 1.0);

    vec2 v = modelSpace.xy + in_pos.xy;
    color = particles[gl_InstanceID].color.rgb;
    uv = in_uv;

    gl_Position = proj * vec4(v, modelSpace.z, 1.0);
}