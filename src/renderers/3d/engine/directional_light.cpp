#include "directional_light.h"

Directional_Light::Directional_Light(glm::vec3 direction) : Light(
        glm::vec3{0.f}), direction_{direction} {}

void Directional_Light::attach_to_shader(Shader *shader) {
    shader->bind();

    shader->set_uniform_3_fv("dir_light.direction", direction_);
    shader->set_uniform_3_fv("dir_light.ambient", ambient_);
    shader->set_uniform_3_fv("dir_light.diffuse", diffuse_);
    shader->set_uniform_3_fv("dir_light.specular", specular_);

    shader->unbind();
}

void Directional_Light::set_direction(glm::vec3 direction) {
    direction_ = direction;
}
