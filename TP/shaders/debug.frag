#version 450

#include "utils.glsl"

layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 out_color;

layout(binding = 0) uniform sampler2D in_texture;

layout(binding = 0) uniform Data {
    FrameData frame;
};

uniform uint depth = 0;

void main()
{
    vec4 color = texelFetch(in_texture, ivec2(gl_FragCoord.xy), 0);
    if (depth != 0)
    {
        color.r *= 5e4;
        color.gb = vec2(color.r);
    }
    out_color = vec4(color.rgb, 1.0);
}