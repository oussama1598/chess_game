#pragma once

#include <glm/glm.hpp>
#include "shader.h"

class Light {
private:
    glm::vec3 position_;
    glm::vec3 color_;
    float intensity_;
    float constant;
    float linear;
    float quadratic;

public:

    Light(glm::vec3 position, glm::vec3 color, float intensity);

    void set_position(glm::vec3 position);

    void attach_to_shader(Shader *shader);
};
