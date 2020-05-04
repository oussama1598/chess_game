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
in vec3 vs_color;
in vec3 vs_normal;


uniform Material material;
uniform PointLight point_light;
uniform DirLight dir_light;

uniform vec3 camera_position;

uniform int sky_box;
uniform samplerCube sky_cube_texture;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 texture_diffuse_color = material.use_texture * texture(material.diffuse_texture, vs_text_coord).rgb;
    vec3 no_texture_diffuse_color = ((material.use_texture + 1) % 2) * material.diffuse;

    vec3 texture_specular_color = material.use_texture * texture(material.specular_texture, vs_text_coord).rgb;
    vec3 no_texture_specular_color = ((material.use_texture + 1) % 2) * material.specular;

    // Combine results
    vec3 ambient = light.ambient * material.ambient * (texture_diffuse_color + no_texture_diffuse_color);
    vec3 diffuse = light.diffuse * diff * (texture_diffuse_color + no_texture_diffuse_color);
    vec3 specular = light.specular * spec * (texture_specular_color + no_texture_specular_color);

    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 texture_diffuse_color = material.use_texture * texture(material.diffuse_texture, vs_text_coord).rgb;
    vec3 no_texture_diffuse_color = ((material.use_texture + 1) % 2) * material.diffuse;

    vec3 texture_specular_color = material.use_texture * texture(material.specular_texture, vs_text_coord).rgb;
    vec3 no_texture_specular_color = ((material.use_texture + 1) % 2) * material.specular;

    // Combine results
    vec3 ambient = light.ambient * material.ambient * (texture_diffuse_color + no_texture_diffuse_color);
    vec3 diffuse = light.diffuse * diff * (texture_diffuse_color + no_texture_diffuse_color);
    vec3 specular = light.specular * spec * (texture_specular_color + no_texture_specular_color);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    if (sky_box == 1)
    {
        FragColor = texture(sky_cube_texture, vs_position);
    } else {
        vec3 norm = normalize(vs_normal);
        vec3 viewDir = normalize(camera_position - vs_position);

        // Directional lighting
        vec3 result = CalcDirLight(dir_light, norm, viewDir);

        // Point lights
        result += CalcPointLight(point_light, norm, vs_position, viewDir);

        FragColor = vec4(result, 1.f);

        vec3 view_vector = normalize(vs_position - camera_position);
        vec3 reflected_vector = reflect(view_vector, normalize(vs_normal));

        vec4 reflected_color = texture(sky_cube_texture, reflected_vector);

        vec3 text_specular = material.use_texture * texture(material.specular_texture, vs_text_coord).rgb;
        vec3 no_texture_specular = ((material.use_texture + 1) % 2) * vec3(0.1f);

        FragColor = mix(FragColor, reflected_color, (text_specular + no_texture_specular).x);
    }
}
