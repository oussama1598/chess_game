#version 440

layout (location = 0) out vec4 FragColor;

in vec2 texture_coordinate;

uniform sampler2D scene_color;
uniform sampler2D bloom_color;
uniform sampler2D emissive_color;

vec3 aces_filmic(vec3 color)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((color * (a * color + b)) /
                 (color * (c * color + d) + e), 0.0, 1.0);
}

void main()
{
    vec3 linear_color = texture(scene_color, texture_coordinate).rgb;
    vec3 bloom = texture(bloom_color, texture_coordinate).rgb;
    vec3 emissive = texture(emissive_color, texture_coordinate).rgb;
    vec3 mapped = aces_filmic((linear_color + emissive * 0.14 + bloom * 0.10) * 1.12);
    mapped = pow(mapped, vec3(1.0 / 2.2));

    vec2 centered = texture_coordinate * 2.0 - 1.0;
    float corner_distance = dot(centered, centered);
    float vignette = 1.0 - smoothstep(0.45, 1.75, corner_distance) * 0.07;
    FragColor = vec4(mapped * vignette, 1.0);
}
