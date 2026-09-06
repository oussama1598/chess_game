#include "material.h"

Material::Material(
        glm::vec3 base_color,
        float roughness,
        float metallic,
        float ao,
        float clearcoat,
        GLint diffuse_texture,
        GLint specular_texture,
        GLint surface_style,
        float albedo_variation,
        float roughness_variation,
        float micro_normal_strength,
        float direct_specular_scale,
        float environment_specular_scale
) : base_color_{base_color}, roughness_{roughness}, metallic_{metallic}, ao_{ao},
    clearcoat_{clearcoat},
    surface_style_{surface_style},
    albedo_variation_{albedo_variation},
    roughness_variation_{roughness_variation},
    micro_normal_strength_{micro_normal_strength},
    direct_specular_scale_{direct_specular_scale},
    environment_specular_scale_{environment_specular_scale},
    diffuse_texture_{diffuse_texture},
    specular_texture_{specular_texture} {

}

void Material::assign_to_shader(Shader &shader) {
    shader.set_uniform_3_fv("material.base_color", base_color_);
    shader.set_uniform_1_f("material.roughness", roughness_);
    shader.set_uniform_1_f("material.metallic", metallic_);
    shader.set_uniform_1_f("material.ao", ao_);
    shader.set_uniform_1_f("material.clearcoat", clearcoat_);
    shader.set_uniform_1_i("material.surface_style", surface_style_);
    shader.set_uniform_1_f("material.albedo_variation", albedo_variation_);
    shader.set_uniform_1_f("material.roughness_variation", roughness_variation_);
    shader.set_uniform_1_f("material.micro_normal_strength", micro_normal_strength_);
    shader.set_uniform_1_f("material.direct_specular_scale", direct_specular_scale_);
    shader.set_uniform_1_f("material.environment_specular_scale",
                           environment_specular_scale_);
    shader.set_uniform_1_i("material.diffuse_texture", diffuse_texture_);
    shader.set_uniform_1_i("material.specular_texture", specular_texture_);
}
