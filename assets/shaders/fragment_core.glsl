#version 440
#define MAX_POINT_LIGHTS 4

out vec4 FragColor;

struct Cell
{
    int i;
    int j;
};

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
uniform PointLight point_light[MAX_POINT_LIGHTS];
uniform DirLight dir_light;

uniform vec3 camera_position;

uniform int sky_box;
uniform samplerCube sky_cube_texture;

// options
uniform int reflection_enabled;
uniform int enabled_directional_lighting;
uniform int enabled_points_lights[MAX_POINT_LIGHTS];

// history
uniform Cell last_from_cell;
uniform Cell last_to_cell;
uniform vec3 history_color;

// flash message
uniform Cell flash_message;
uniform vec3 error_color;

// guides
uniform int guides_count;
uniform Cell guides[32];
uniform vec3 guides_color;

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

        if (material.use_texture == 1)
        {
            int x_row = int((vs_text_coord.x - 0.7034) / 0.036);
            int y_row = int((vs_text_coord.y - 0.70285) / 0.0353);

            int cell = 8 - (y_row + 48);

            for (int i = 0; i < guides_count; i++)
            {
                if (guides[i].i == cell && guides[i].j == x_row)
                {
                    FragColor = vec4(guides_color, 1.f);
                    return;
                }
            }

            if (flash_message.i == cell && flash_message.j == x_row)
            {
                FragColor = vec4(error_color, 1.f);
                return;
            }

            if ((last_from_cell.i == cell && last_from_cell.j == x_row) || (last_to_cell.i == cell && last_to_cell.j == x_row)){
                FragColor = vec4(history_color, 1.f);

                return;
            }

        }

        vec3 norm = normalize(vs_normal);
        vec3 viewDir = normalize(camera_position - vs_position);

        // Directional lighting
        vec3 result = enabled_directional_lighting * CalcDirLight(dir_light, norm, viewDir);

        // Point lights
        for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
        {
            result += enabled_points_lights[i] * CalcPointLight(point_light[i], norm, vs_position, viewDir);
        }

        FragColor = vec4(result, 1.f);

        vec3 view_vector = normalize(vs_position - camera_position);
        vec3 reflected_vector = reflect(view_vector, normalize(vs_normal));

        vec4 reflected_color = texture(sky_cube_texture, reflected_vector);

        vec3 text_specular = reflection_enabled * material.use_texture * texture(material.specular_texture, vs_text_coord).rgb;
        vec3 no_texture_specular = reflection_enabled * ((material.use_texture + 1) % 2) * vec3(0.1f);

        FragColor = mix(FragColor, reflected_color, (text_specular + no_texture_specular).x);
    }
}
