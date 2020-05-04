#include "light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
             glm::vec3 specular) :
        position_{position}, ambient_{ambient}, diffuse_{diffuse},
        specular_{specular} {}

void Light::set_position(glm::vec3 position) {
    position_ = position;
}

void Light::set_direction(glm::vec3) {}
