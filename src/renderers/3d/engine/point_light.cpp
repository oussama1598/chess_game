#include "point_light.h"

Point_Light::Point_Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                         glm::vec3 specular, float constant, float linear, float quadratic) : Light(
        position, ambient, diffuse, specular), constant_{constant}, linear_{linear}, quadratic_{
        quadratic} {}

void Point_Light::attach_to_shader(Shader *shader) {
    shader->bind();

    shader->set_uniform_3_fv("point_light.position", position_);
    shader->set_uniform_3_fv("point_light.ambient", ambient_);
    shader->set_uniform_3_fv("point_light.diffuse", diffuse_);
    shader->set_uniform_3_fv("point_light.specular", specular_);

    shader->set_uniform_1_f("point_light.constant", constant_);
    shader->set_uniform_1_f("point_light.linear", linear_);
    shader->set_uniform_1_f("point_light.quadratic", quadratic_);

    shader->unbind();
}
