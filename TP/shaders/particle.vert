#version 450

#include "utils.glsl"

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;
layout(location = 3) in vec4 in_tangent_bitangent_sign;
layout(location = 4) in vec3 in_color;

// layout(location = 0) out vec3 out_normal;
// layout(location = 1) out vec2 out_uv;
// layout(location = 2) out vec3 out_color;
// layout(location = 3) out vec3 out_position;
// layout(location = 4) out vec3 out_tangent;
// layout(location = 5) out vec3 out_bitangent;

layout(binding = 0) uniform Data {
    FrameData frame;
};

layout(std430, binding = 1) buffer ParticleSSBO {
    Particle particles[];
};

// uniform mat4 model;
uniform float dt = 0.01f;

out vec3 color;

// mat4 proj = mat4(2.0 / 1600, 0.0, 0.0, 0.0,
//                            0.0, 2.0 / -900, 0.0, 0.0,
//                            0.0, 0.0, -1.0, 0.0,
//                            -1.0, 1.0, 0.0, 1.0);

void main() {
    



    //mat4 model = particles[gl_InstanceID].transform;
    mat4 view = frame.camera.view;
    mat4 proj = frame.camera.proj;

    vec4 modelSpace = view * vec4(particles[gl_InstanceID].center, 1.0);

    vec2 v = modelSpace.xy + in_pos.xy;
    gl_Position = proj * vec4(v, modelSpace.z, 1.0);

    // vec4 camera_right = view * vec4(1.0, 0.0, 0.0, 0.0);
    // vec4 camera_up = view * vec4(0.0, 1.0, 0.0, 0.0);
    
    // vec4 center = vec4(particles[gl_InstanceID].center, 1.0) * particles[gl_InstanceID].transform;

    // vec3 position = center.xyz + camera_right.xyz * in_pos.x + camera_up.xyz * in_pos.y;
    
    // const vec4 position = model_view * vec4(in_pos, 1.0);

    particles[gl_InstanceID].color.g = mod(particles[gl_InstanceID].color.g + dt, 1.0);
    color = particles[gl_InstanceID].color.rgb;
}