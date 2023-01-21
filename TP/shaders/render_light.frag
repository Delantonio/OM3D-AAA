#version 450

#include "utils.glsl"

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_color;
layout(location = 3) in vec3 in_position;
layout(location = 4) in vec3 in_tangent;
layout(location = 5) in vec3 in_bitangent;

layout(binding = 0) uniform sampler2D in_texture;
layout(binding = 1) uniform sampler2D in_normal_texture;
layout(binding = 2) uniform sampler2D in_depth_texture;

layout(binding = 0) uniform Data {
    FrameData frame;
};

vec3 unproject(vec2 uv, float depth, mat4 inv_viewproj) {
    const vec3 ndc = vec3(uv * 2.0 - vec2(1.0), depth);
    const vec4 p = inv_viewproj * vec4(ndc, 1.0);
    return p.xyz / p.w;
}

const vec3 ambient = vec3(0.0);

uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_radius;

void main()
{
    // const float depth = texelFetch(in_depth_texture, ivec2(gl_FragCoord.xy), 0).r;
    // const vec3 normal = unpack_normal_map(texelFetch(in_normal_texture, ivec2(gl_FragCoord.xy), 0).xy);

    // vec3 position = unproject(in_uv, depth, inverse(frame.camera.view_proj));

    vec3 acc = ambient;
    
    const vec3 to_light = (light_position - in_position);
    const float dist = length(to_light);
    const vec3 light_vec = to_light / dist;

    const float NoL = dot(light_vec, in_normal);
    const float att = attenuation(dist, light_radius);
    if(NoL <= 0.0 || att <= 0.0) {
        acc += 0.0;
    }
    else
    {
        acc += light_color * (NoL * att);
    }

    // vec4 color = texelFetch(in_texture, ivec2(gl_FragCoord.xy), 0);
    out_color = vec4(acc, 1.0);
}