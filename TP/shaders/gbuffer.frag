#version 450

#include "utils.glsl"


layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_color;
layout(location = 3) in vec3 in_position;
layout(location = 4) in vec3 in_tangent;
layout(location = 5) in vec3 in_bitangent;

layout(location = 0) out vec3 g_color;
layout(location = 1) out vec3 g_normal;

layout(binding = 0) uniform sampler2D in_texture;
layout(binding = 1) uniform sampler2D in_normal_texture;

// layout(binding = 0) uniform Data {
//     FrameData frame;
// };

// layout(binding = 1) buffer PointLights {
//     PointLight point_lights[];
// };

void main()
{
#ifdef NORMAL_MAPPED
    const vec3 normal_map = unpack_normal_map(texture(in_normal_texture, in_uv).xy);
    const vec3 normal = normal_map.x * in_tangent +
                        normal_map.y * in_bitangent +
                        normal_map.z * in_normal;
#else
    const vec3 normal = in_normal;
#endif

    // change interval from [-1, 1] to [0, 1]
    g_normal = (normal + 1.0) / 2.0;
    g_color = in_color;
    
#ifdef TEXTURED
    g_color *= texture(in_texture, in_uv).xyz;
#endif

}