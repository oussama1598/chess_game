#version 440
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    int use_texture;
    sampler2D diffuse_texture;
    sampler2D specular_texture;
};

in vec3 vs_position;
in vec2 vs_text_coord;
in vec3 vs_color;
in vec3 vs_normal;

uniform Material material;
uniform int use_texture;
uniform vec3 camera_position;
uniform float object_index;

void main()
{
    if (material.use_texture == 1) {

        int x_row = int((vs_text_coord.x - 0.703) / 0.036);
        int y_row = int((vs_text_coord.y - 0.709) / 0.0353);

        int cell = 8 - (y_row + 48);

        FragColor = vec4(x_row/255.f, cell/255.f, 1.f, 1.f);
    }
    else {
        FragColor = vec4(object_index/255, 0.f, 0.f, 1.f);
    }
}
