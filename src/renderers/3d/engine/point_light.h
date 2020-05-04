#pragma once

#include "light.h"

class Point_Light : public Light {
private:
    float constant_;
    float linear_;
    float quadratic_;

public:
    Point_Light(glm::vec3 position, float constant = 1.0f,
                float linear = 0.09f,
                float quadratic = 0.032f);


    void attach_to_shader(Shader *shader) override;
};
