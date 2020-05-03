#include "light.h"

Light::Light(glm::vec3 position, glm::vec3 color, float intensity)
        : position_{position}, color_{color}, intensity_{intensity} {}

void Light::set_position(glm::vec3 position) {
    position_ = position;
}

void Light::attach_to_shader(Shader *shader) {
    shader->bind();

    shader->set_uniform_3_fv("point_light.position", position_);
    shader->set_uniform_3_fv("point_light.color", color_);
    shader->set_uniform_1_f("point_light.intensity", intensity_);

    shader->unbind();
}
