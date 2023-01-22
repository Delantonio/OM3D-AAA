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

layout(binding = 2) buffer PointLights {
    PointLight point_lights[];
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

    vec3 acc = ambient;
    
    // TODO: frame.numParticles instead of point_light_count
    for(uint i = 0; i != frame.point_light_count; ++i) {
        PointLight light = point_lights[i];
        const vec3 to_light = (light.position - position);
        const float dist = length(to_light);
        const vec3 light_vec = to_light / dist;

        const float NoL = dot(light_vec, normal);
        const float att = attenuation(dist, light.radius);
        if(NoL <= 0.0 || att <= 0.0) {
            continue;
        }
        acc += light.color * (NoL * att);
    }
    out_color = vec4(acc, 1.0);
    bright = vec4(ambient, 1.0);
    float brightness = dot(acc, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        bright = out_color;
}