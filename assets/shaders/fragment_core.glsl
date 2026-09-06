#version 440
#define MAX_POINT_LIGHTS 4

layout (location = 0) out vec4 SceneColor;
layout (location = 1) out vec4 EmissiveColor;

struct Cell {
    int i;
    int j;
};

struct Material {
    vec3 base_color;
    float roughness;
    float metallic;
    float ao;
    float clearcoat;
    int surface_style;
    float albedo_variation;
    float roughness_variation;
    float micro_normal_strength;
    float direct_specular_scale;
    float environment_specular_scale;

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
in vec3 vs_object_position;
in vec2 vs_text_coord;
in vec3 vs_color;
in vec3 vs_normal;
in vec4 vs_light_space_position;

uniform Material material;
uniform PointLight point_light[MAX_POINT_LIGHTS];
uniform DirLight dir_light;
uniform vec3 camera_position;

uniform int sky_box;
uniform samplerCube sky_cube_texture;

uniform int reflection_enabled;
uniform int enabled_directional_lighting;
uniform int enabled_points_lights[MAX_POINT_LIGHTS];
uniform float environment_yaw_degrees;

uniform Cell last_from_cell;
uniform Cell last_to_cell;
uniform vec3 history_color;

uniform Cell flash_message;
uniform vec3 error_color;

uniform int guides_count;
uniform Cell guides[32];
uniform int guide_capture[32];
uniform vec3 guides_color;

uniform Cell selected_cell;
uniform Cell hover_cell;
uniform vec3 selected_color;
uniform vec3 hover_color;
uniform Cell landing_cell;
uniform float landing_progress;

uniform int is_chess_board;
uniform vec2 texture_scale;
uniform int shadows_enabled;
uniform sampler2D shadow_map;
uniform float surface_seed;

const float PI = 3.14159265359;

vec3 base_color()
{
    if (material.use_texture == 1) {
        vec3 texture_color = pow(
                texture(material.diffuse_texture, vs_text_coord * texture_scale).rgb,
                vec3(2.2));
        return texture_color * material.base_color;
    }
    return material.base_color;
}

float surface_roughness()
{
    if (material.use_texture == 1) {
        float specular_level = texture(
                material.specular_texture, vs_text_coord * texture_scale).r;
        float texture_factor = 0.75 + (1.0 - specular_level) * 0.50;
        return clamp(material.roughness * texture_factor, 0.08, 1.0);
    }
    return clamp(material.roughness, 0.08, 1.0);
}

float hash_position(vec3 position)
{
    position = fract(position * 0.1031);
    position += dot(position, position.yzx + 33.33);
    return fract((position.x + position.y) * position.z);
}

float value_noise(vec3 position)
{
    vec3 cell = floor(position);
    vec3 local_position = fract(position);
    local_position = local_position * local_position *
                     (3.0 - 2.0 * local_position);

    float x00 = mix(hash_position(cell + vec3(0, 0, 0)),
                    hash_position(cell + vec3(1, 0, 0)), local_position.x);
    float x10 = mix(hash_position(cell + vec3(0, 1, 0)),
                    hash_position(cell + vec3(1, 1, 0)), local_position.x);
    float x01 = mix(hash_position(cell + vec3(0, 0, 1)),
                    hash_position(cell + vec3(1, 0, 1)), local_position.x);
    float x11 = mix(hash_position(cell + vec3(0, 1, 1)),
                    hash_position(cell + vec3(1, 1, 1)), local_position.x);
    float y0 = mix(x00, x10, local_position.y);
    float y1 = mix(x01, x11, local_position.y);
    return mix(y0, y1, local_position.z);
}

float three_octave_noise(vec3 position)
{
    return value_noise(position) * 0.5714 +
           value_noise(position * 2.03 + 9.2) * 0.2857 +
           value_noise(position * 4.11 + 21.7) * 0.1429;
}

vec3 perturb_styled_normal(vec3 normal, float height, float strength,
                           float view_distance)
{
    float distance_fade = 1.0 - smoothstep(9.0, 14.0, view_distance);
    if (strength <= 0.0 || distance_fade <= 0.0) return normal;

    vec3 position_dx = dFdx(vs_position);
    vec3 position_dy = dFdy(vs_position);
    float height_dx = dFdx(height);
    float height_dy = dFdy(height);
    vec3 tangent_x = cross(position_dy, normal);
    vec3 tangent_y = cross(normal, position_dx);
    float determinant = dot(position_dx, tangent_x);
    vec3 surface_gradient = sign(determinant) *
                            (height_dx * tangent_x + height_dy * tangent_y) /
                            max(abs(determinant), 0.000001);
    return normalize(normal - surface_gradient * strength * distance_fade);
}

void apply_surface_style(inout vec3 albedo, inout float roughness,
                         inout vec3 normal, float view_distance)
{
    if (material.surface_style == 0) return;

    vec3 seed_offset = vec3(surface_seed * 13.7,
                            surface_seed * 7.1,
                            surface_seed * 19.3);
    float pattern = 0.5;
    float micro_height = 0.5;

    if (material.surface_style == 1) {
        pattern = three_octave_noise(vs_object_position * 3.2 + seed_offset);
        float vein = smoothstep(
                0.76, 0.94,
                three_octave_noise(vs_object_position * vec3(2.2, 5.4, 2.2) +
                                   seed_offset.yzx));
        albedo *= mix(vec3(1.0), vec3(1.012, 0.998, 0.980), vein);
        micro_height = mix(
                pattern,
                value_noise(vs_object_position * 12.0 + seed_offset.zxy),
                0.10);
    } else {
        vec3 grain_position = vs_object_position * vec3(18.0, 3.0, 18.0) +
                              seed_offset;
        float grain_noise = value_noise(grain_position);
        float grain_wave = sin((vs_object_position.x + vs_object_position.z) * 54.0 +
                               vs_object_position.y * 5.0 + grain_noise * 3.2 +
                               surface_seed * 6.2831853) * 0.5 + 0.5;
        pattern = mix(grain_noise, grain_wave, 0.38);
        micro_height = pattern;
    }

    float signed_pattern = clamp((pattern - 0.5) * 2.0, -1.0, 1.0);
    albedo *= 1.0 + signed_pattern * material.albedo_variation;
    roughness = clamp(roughness + signed_pattern * material.roughness_variation,
                      0.08, 1.0);
    normal = perturb_styled_normal(normal, micro_height,
                                   material.micro_normal_strength, view_distance);
}

float distribution_ggx(vec3 normal, vec3 halfway_direction, float roughness)
{
    float alpha = roughness * roughness;
    float alpha_squared = alpha * alpha;
    float n_dot_h = max(dot(normal, halfway_direction), 0.0);
    float denominator = n_dot_h * n_dot_h * (alpha_squared - 1.0) + 1.0;
    return alpha_squared / max(PI * denominator * denominator, 0.000001);
}

float geometry_schlick_ggx(float n_dot_direction, float roughness)
{
    float value = roughness + 1.0;
    float k = value * value / 8.0;
    return n_dot_direction / max(n_dot_direction * (1.0 - k) + k, 0.000001);
}

float geometry_smith(vec3 normal, vec3 view_direction, vec3 light_direction, float roughness)
{
    float n_dot_v = max(dot(normal, view_direction), 0.0);
    float n_dot_l = max(dot(normal, light_direction), 0.0);
    return geometry_schlick_ggx(n_dot_v, roughness) *
           geometry_schlick_ggx(n_dot_l, roughness);
}

vec3 fresnel_schlick(float cosine, vec3 reflectance_at_normal)
{
    return reflectance_at_normal + (1.0 - reflectance_at_normal) *
           pow(clamp(1.0 - cosine, 0.0, 1.0), 5.0);
}

vec3 fresnel_schlick_roughness(float cosine, vec3 reflectance_at_normal, float roughness)
{
    return reflectance_at_normal +
           (max(vec3(1.0 - roughness), reflectance_at_normal) - reflectance_at_normal) *
           pow(clamp(1.0 - cosine, 0.0, 1.0), 5.0);
}

vec3 evaluate_brdf(vec3 albedo, vec3 normal, vec3 view_direction,
                   vec3 light_direction, vec3 radiance, float roughness,
                   float metallic, float clearcoat)
{
    vec3 halfway_direction = normalize(view_direction + light_direction);
    float n_dot_v = max(dot(normal, view_direction), 0.001);
    float n_dot_l = max(dot(normal, light_direction), 0.0);
    float h_dot_v = max(dot(halfway_direction, view_direction), 0.0);

    vec3 f0 = mix(vec3(0.04), albedo, metallic);
    vec3 fresnel = fresnel_schlick(h_dot_v, f0);
    float distribution = distribution_ggx(normal, halfway_direction, roughness);
    float geometry = geometry_smith(normal, view_direction, light_direction, roughness);
    vec3 specular = distribution * geometry * fresnel /
                    max(4.0 * n_dot_v * n_dot_l, 0.001);

    vec3 diffuse_weight = (vec3(1.0) - fresnel) * (1.0 - metallic);

    float coat_roughness = mix(0.16, 0.09, clearcoat);
    float coat_distribution = distribution_ggx(normal, halfway_direction, coat_roughness);
    float coat_geometry = geometry_smith(
            normal, view_direction, light_direction, coat_roughness);
    vec3 coat_fresnel = fresnel_schlick(h_dot_v, vec3(0.04));
    vec3 coat_specular = coat_distribution * coat_geometry * coat_fresnel /
                         max(4.0 * n_dot_v * n_dot_l, 0.001);

    vec3 direct_specular = specular + clearcoat * 0.20 * coat_specular;
    return (diffuse_weight * albedo / PI +
            direct_specular * clamp(material.direct_specular_scale, 0.0, 1.0)) *
           radiance * n_dot_l;
}

float directional_shadow(vec4 light_space_position, vec3 normal, vec3 light_direction)
{
    const vec2 blocker_offsets[8] = vec2[](
        vec2(-0.7071, -0.7071), vec2(0.0, -1.0),
        vec2(0.7071, -0.7071), vec2(1.0, 0.0),
        vec2(0.7071, 0.7071), vec2(0.0, 1.0),
        vec2(-0.7071, 0.7071), vec2(-1.0, 0.0)
    );
    const vec2 poisson_offsets[12] = vec2[](
        vec2(-0.326, -0.406), vec2(-0.840, -0.074),
        vec2(-0.696, 0.457), vec2(-0.203, 0.621),
        vec2(0.192, -0.824), vec2(0.473, -0.481),
        vec2(0.519, 0.165), vec2(0.185, 0.680),
        vec2(0.768, 0.512), vec2(0.886, -0.255),
        vec2(-0.064, -0.191), vec2(-0.548, -0.790)
    );

    vec3 projected = light_space_position.xyz / light_space_position.w;
    projected = projected * 0.5 + 0.5;
    if (projected.z <= 0.0 || projected.z >= 1.0) return 0.0;
    if (projected.x <= 0.0 || projected.x >= 1.0 ||
        projected.y <= 0.0 || projected.y >= 1.0) return 0.0;

    float bias = max(0.0022 * (1.0 - dot(normal, light_direction)), 0.00045);
    vec2 texel_size = 1.0 / vec2(textureSize(shadow_map, 0));
    float receiver_depth = projected.z - bias;
    float blocker_depth_sum = 0.0;
    float blocker_count = 0.0;
    for (int sample_index = 0; sample_index < 8; ++sample_index) {
        float sample_depth = texture(
                shadow_map,
                projected.xy + blocker_offsets[sample_index] * texel_size * 3.0).r;
        if (sample_depth < receiver_depth) {
            blocker_depth_sum += sample_depth;
            blocker_count += 1.0;
        }
    }
    if (blocker_count == 0.0) return 0.0;

    float average_blocker_depth = blocker_depth_sum / blocker_count;
    float depth_separation = max(receiver_depth - average_blocker_depth, 0.0);
    float filter_radius = clamp(1.25 + depth_separation * 90.0, 1.25, 4.25);

    float shadow = 0.0;
    for (int sample_index = 0; sample_index < 12; ++sample_index) {
        float sample_depth = texture(
                shadow_map,
                projected.xy + poisson_offsets[sample_index] * texel_size * filter_radius).r;
        shadow += receiver_depth > sample_depth ? 1.0 : 0.0;
    }
    return shadow / 12.0;
}

vec3 point_light_color(PointLight light, vec3 albedo, vec3 normal, vec3 position,
                       vec3 view_direction, float roughness, float metallic,
                       float clearcoat)
{
    vec3 light_direction = normalize(light.position - position);
    float distance_to_light = length(light.position - position);
    float attenuation = 1.0 / (light.constant + light.linear * distance_to_light +
                               light.quadratic * distance_to_light * distance_to_light);
    return evaluate_brdf(albedo, normal, view_direction, light_direction,
                         light.diffuse * attenuation, roughness, metallic, clearcoat);
}

vec3 rotate_environment_direction(vec3 direction)
{
    float angle = radians(environment_yaw_degrees);
    float cosine = cos(angle);
    float sine = sin(angle);
    return vec3(cosine * direction.x - sine * direction.z,
                direction.y,
                sine * direction.x + cosine * direction.z);
}

void main()
{
    EmissiveColor = vec4(0.0);

    if (sky_box == 1) {
        vec3 direction = rotate_environment_direction(normalize(vs_position));
        vec3 environment = pow(
                textureLod(sky_cube_texture, direction, 1.5).rgb,
                vec3(2.2));
        SceneColor = vec4(environment * 0.4643, 1.0);
        return;
    }

    bool history_overlay = false;
    bool hover_overlay = false;
    bool selected_overlay = false;
    bool flash_overlay = false;
    bool guide_overlay = false;
    bool capture_guide = false;
    bool landing_overlay = false;
    vec2 cell_position = vec2(0.5);

    if (is_chess_board == 1) {
        int column = clamp(int((vs_text_coord.x - 0.7034) / 0.036), 0, 7);
        int row = clamp(int((-vs_text_coord.y - 0.70915) / 0.0353), 0, 7);
        cell_position = vec2(
                fract((vs_text_coord.x - 0.7034) / 0.036),
                fract((-vs_text_coord.y - 0.70915) / 0.0353));

        if ((last_from_cell.i == row && last_from_cell.j == column) ||
            (last_to_cell.i == row && last_to_cell.j == column)) {
            history_overlay = true;
        }

        if (hover_cell.i == row && hover_cell.j == column) {
            hover_overlay = true;
        }

        for (int i = 0; i < guides_count; ++i) {
            if (guides[i].i == row && guides[i].j == column) {
                guide_overlay = true;
                capture_guide = guide_capture[i] == 1;
            }
        }

        if (selected_cell.i == row && selected_cell.j == column) {
            selected_overlay = true;
        }

        if (flash_message.i == row && flash_message.j == column) {
            flash_overlay = true;
        }

        landing_overlay = landing_cell.i == row && landing_cell.j == column &&
                          landing_progress >= 0.0 && landing_progress < 1.0;
    }

    vec3 normal = normalize(vs_normal);
    vec3 view_direction = normalize(camera_position - vs_position);
    vec3 albedo = base_color();
    float roughness = surface_roughness();
    apply_surface_style(albedo, roughness, normal,
                        length(camera_position - vs_position));
    float metallic = clamp(material.metallic, 0.0, 1.0);
    float ao = clamp(material.ao, 0.0, 1.0);
    float clearcoat = clamp(material.clearcoat, 0.0, 1.0);
    float n_dot_v = max(dot(normal, view_direction), 0.0);
    vec3 f0 = mix(vec3(0.04), albedo, metallic);

    vec3 result = albedo * dir_light.ambient * ao;

    if (reflection_enabled == 1) {
        vec3 reflected_direction = rotate_environment_direction(
                reflect(-view_direction, normal));
        vec3 environment = pow(
                textureLod(sky_cube_texture, reflected_direction, roughness * 5.0).rgb,
                vec3(2.2));
        vec3 environment_fresnel = fresnel_schlick_roughness(
                n_dot_v, f0, roughness);
        result += environment * environment_fresnel *
                  (0.16 + clearcoat * 0.06) * ao *
                  max(material.environment_specular_scale, 0.0);
    }

    if (enabled_directional_lighting == 1) {
        vec3 light_direction = normalize(-dir_light.direction);
        float shadow = shadows_enabled == 1
                       ? directional_shadow(vs_light_space_position, normal, light_direction)
                       : 0.0;
        result += (1.0 - shadow) * evaluate_brdf(
                albedo, normal, view_direction, light_direction, dir_light.diffuse,
                roughness, metallic, clearcoat);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS; ++i) {
        if (enabled_points_lights[i] == 1) {
            vec3 point_contribution = point_light_color(
                    point_light[i], albedo, normal, vs_position,
                    view_direction, roughness, metallic, clearcoat);

            // Preserve the cool rear-right rim on pieces and salon geometry while
            // preventing it from reading as a cyan spotlight on the board finish.
            if (is_chess_board == 1 && i == 2) {
                point_contribution *= 0.25;
            }

            result += point_contribution;
        }
    }

    if (history_overlay) {
        result = mix(result, history_color, 0.18);
    }

    if (is_chess_board == 1) {
        float edge_distance = min(min(cell_position.x, 1.0 - cell_position.x),
                                  min(cell_position.y, 1.0 - cell_position.y));
        float edge_aa = max(fwidth(edge_distance) * 1.25, 0.002);
        float radius = length(cell_position - vec2(0.5));
        float radial_aa = max(fwidth(radius) * 1.25, 0.002);

        if (hover_overlay) {
            float hover_line = 1.0 - smoothstep(0.016, 0.016 + edge_aa,
                                                abs(edge_distance - 0.060));
            result = mix(result, hover_color, hover_line * 0.24);
        }

        // Capture targets belong to the physical scene and are allowed to be
        // occluded by their piece. Empty targets are drawn in a restrained
        // late pass so a grazing camera angle cannot erase move information.
        if (guide_overlay && capture_guide) {
            float outer = 1.0 - smoothstep(0.235, 0.235 + radial_aa, radius);
            float inner = smoothstep(0.195 - radial_aa, 0.195, radius);
            float guide_shape = outer * inner;
            result = mix(result, guides_color, guide_shape * 0.62);
            EmissiveColor.rgb += guides_color * guide_shape * 1.65;
            EmissiveColor.a = max(EmissiveColor.a, guide_shape);
        }

        if (selected_overlay) {
            float selected_line = 1.0 - smoothstep(0.016, 0.016 + edge_aa,
                                                   abs(edge_distance - 0.050));
            result = mix(result, selected_color, selected_line * 0.40);
            EmissiveColor.rgb += selected_color * selected_line * 0.84;
            EmissiveColor.a = max(EmissiveColor.a, selected_line);
        }

        if (landing_overlay) {
            float ring_radius = mix(0.10, 0.36, landing_progress);
            float landing_ring = 1.0 - smoothstep(0.024, 0.024 + radial_aa,
                                                  abs(radius - ring_radius));
            float landing_fade = 1.0 - smoothstep(0.12, 1.0, landing_progress);
            result = mix(result, guides_color, landing_ring * landing_fade * 0.55);
            EmissiveColor.rgb += guides_color * landing_ring * landing_fade * 2.0;
            EmissiveColor.a = max(EmissiveColor.a, landing_ring * landing_fade);
        }
    }

    if (flash_overlay) {
        result = mix(result, error_color, 0.62);
    }

    SceneColor = vec4(max(result, vec3(0.0)), 1.0);
}
