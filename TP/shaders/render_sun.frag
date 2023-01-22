#version 450

#include "utils.glsl"

layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 bright;

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

void main()
{
    const float depth = texelFetch(in_depth_texture, ivec2(gl_FragCoord.xy), 0).r;
    const vec3 normal = unpack_normal_map(texelFetch(in_normal_texture, ivec2(gl_FragCoord.xy), 0).xy);

    vec3 position = unproject(in_uv, depth, inverse(frame.camera.view_proj));

    vec3 acc = frame.sun_color * max(0.0, dot(frame.sun_dir, normal)) + ambient;
    
    vec4 color = texelFetch(in_texture, ivec2(gl_FragCoord.xy), 0);
    out_color = vec4(color.rgb * acc, 1.0);
    bright = vec4(ambient, 1.0);
}