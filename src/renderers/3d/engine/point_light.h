#pragma once

#include "light.h"

class Point_Light : public Light {
private:
    float constant_;
    float linear_;
    float quadratic_;

public:
    Point_Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                glm::vec3 specular, float constant = 1.0f,
                float linear = 0.5f,
                float quadratic = 0.032f);


    void attach_to_shader(Shader *shader) override;
};
