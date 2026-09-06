#version 440

layout (location = 0) out vec4 FragColor;

in vec2 texture_coordinate;

uniform sampler2D source_image;
uniform vec2 blur_direction;

void main()
{
    vec2 texel = 1.0 / vec2(textureSize(source_image, 0));
    vec3 color = texture(source_image, texture_coordinate).rgb * 0.227027;
    color += texture(source_image,
                     texture_coordinate + blur_direction * texel * 1.384615).rgb * 0.316216;
    color += texture(source_image,
                     texture_coordinate - blur_direction * texel * 1.384615).rgb * 0.316216;
    color += texture(source_image,
                     texture_coordinate + blur_direction * texel * 3.230769).rgb * 0.070270;
    color += texture(source_image,
                     texture_coordinate - blur_direction * texel * 3.230769).rgb * 0.070270;
    FragColor = vec4(color, 1.0);
}
