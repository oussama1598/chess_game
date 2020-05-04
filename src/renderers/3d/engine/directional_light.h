#pragma once

#include "light.h"

class Directional_Light : public Light {
private:
    glm::vec3 direction_;

public:
    Directional_Light(glm::vec3 direction);

    void set_direction(glm::vec3 direction) override;

    void attach_to_shader(Shader *shader) override;
};
