#version 440

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D diffuse_texture;
    sampler2D specular_texture;
};

struct PointLight
{
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_text_coord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform PointLight point_light;
uniform vec3 camera_position;

vec3 calculate_ambient_light()
{
    return material.ambient;
}

vec3 calculate_diffuse_light()
{
    vec3 position_to_light_direction_vector = normalize(point_light.position - vs_position);
    float diffuse_rate = clamp(dot(position_to_light_direction_vector, normalize(vs_normal)), 0, 1);

    return material.diffuse * diffuse_rate;
}

vec3 calculate_specular_light()
{
    vec3 light_to_position_direction_vector = normalize(vs_position - point_light.position);
    vec3 reflect_direction_vector = normalize(reflect(light_to_position_direction_vector, normalize(vs_normal)));
    vec3 position_to_view_direction_vector = normalize(camera_position - vs_position);
    float specular_rate = pow(max(dot(position_to_view_direction_vector, reflect_direction_vector), 0), 30);

    return material.specular * specular_rate * texture(material.specular_texture, vs_text_coord).rgb;
}

void main() {
    //    fs_color = vec4(vs_color, 1.f);

    // Ambient light
    vec3 ambient_final = calculate_ambient_light();

    // diffuse light
    vec3 diffuse_final = calculate_diffuse_light();

    // Specular light
    vec3 specular_final = calculate_specular_light();

    // Attenuation
    float distance = length(point_light.position - vs_position);
    // constant linear quadratic
    float attenuation = 1.f / (1.f + 0.045f * distance + 0.0075f * (distance * distance));

    // final light
    ambient_final *= attenuation;
    diffuse_final *= attenuation;
    specular_final *= attenuation;

    //    fs_color = texture(material.diffuse_texture, vs_text_coord)
    //    * (vec4(ambient_final, 1.f) + vec4(diffuse_final, 1.f) + vec4(specular_final, 1.f));

    fs_color = vec4(vs_color, 1.f)
    * (vec4(ambient_final, 1.f) + vec4(diffuse_final, 1.f) + vec4(specular_final, 1.f));
}
