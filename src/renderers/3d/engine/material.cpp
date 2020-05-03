#include "material.h"

Material::Material(
        glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        GLint diffuse_texture,
        GLint specular_texture
) : ambient_{ambient}, diffuse_{diffuse}, specular_{specular}, diffuse_texture_{diffuse_texture},
    specular_texture_{specular_texture} {

}

void Material::assign_to_shader(Shader &shader) {
    shader.set_uniform_3_fv("material.ambient", ambient_);
    shader.set_uniform_3_fv("material.diffuse", diffuse_);
    shader.set_uniform_3_fv("material.specular", specular_);

    shader.set_uniform_1_i("material.diffuse_texture", diffuse_texture_);
    shader.set_uniform_1_i("material.specular_texture", specular_texture_);
}
