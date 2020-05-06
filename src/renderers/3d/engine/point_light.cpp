#include "point_light.h"

Point_Light::Point_Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                         glm::vec3 specular, float constant, float linear, float quadratic) : Light(
        position, ambient, diffuse, specular), constant_{constant}, linear_{linear}, quadratic_{
        quadratic} {}

void Point_Light::attach_to_shader(Shader *shader) {
    shader->bind();

    std::string light_uniform = "point_light[" + std::to_string(light_index) + "].";

    shader->set_uniform_3_fv(light_uniform + "position", position_);
    shader->set_uniform_3_fv(light_uniform + "ambient", ambient_);
    shader->set_uniform_3_fv(light_uniform + "diffuse", diffuse_);
    shader->set_uniform_3_fv(light_uniform + "specular", specular_);

    shader->set_uniform_1_f(light_uniform + "constant", constant_);
    shader->set_uniform_1_f(light_uniform + "linear", linear_);
    shader->set_uniform_1_f(light_uniform + "quadratic", quadratic_);

    shader->unbind();
}
