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

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vs_position;
in vec2 vs_text_coord;
in vec3 vs_normal;


uniform Material material;
uniform PointLight point_light;
uniform DirLight dir_light;

uniform vec3 camera_position;

vec3 CalcDirLight()
{
    vec3 lightDir = normalize(dir_light.direction);

    // ambient
    vec3 ambient = point_light.ambient * material.ambient;

    // diffuse shading
    float diffuse_rate = clamp(dot(lightDir, normalize(vs_normal)), 0, 1);
    vec3 diffuse = material.diffuse * diffuse_rate * point_light.diffuse;

    // specular shading
    vec3 reflect_direction_vector = normalize(reflect(-lightDir, normalize(vs_normal)));
    vec3 position_to_view_direction_vector = normalize(camera_position - vs_position);
    float specular_rate = pow(max(dot(position_to_view_direction_vector, reflect_direction_vector), 0), material.shininess);
    vec3 specular = point_light.specular * (material.specular * specular_rate);

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight()
{
    // ambient
    vec3 ambient = point_light.ambient * material.ambient;

    // diffuse
    vec3 position_to_light_direction_vector = normalize(point_light.position - vs_position);
    float diffuse_rate = clamp(dot(position_to_light_direction_vector, normalize(vs_normal)), 0, 1);
    vec3 diffuse = material.diffuse * diffuse_rate * point_light.diffuse;

    // specular
    vec3 light_to_position_direction_vector = normalize(vs_position - point_light.position);
    vec3 reflect_direction_vector = normalize(reflect(light_to_position_direction_vector, normalize(vs_normal)));
    vec3 position_to_view_direction_vector = normalize(camera_position - vs_position);
    float specular_rate = pow(max(dot(position_to_view_direction_vector, reflect_direction_vector), 0), material.shininess);
    vec3 specular = point_light.specular * (material.specular * specular_rate);

    // attenuation
    float distance = length(point_light.position - vs_position);
    float attenuation = 1.0 / (point_light.constant + point_light.linear * distance + point_light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{

    vec3 color = CalcDirLight() + CalcPointLight();

    vec4 text = material.use_texture * texture(material.diffuse_texture, vs_text_coord);
    vec4 no_texture_color = ((material.use_texture + 1) % 2) * vec4(1.f);

    FragColor = (text + no_texture_color) * vec4(color, 1.f);
}
