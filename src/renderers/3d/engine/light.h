#pragma once

#include <glm/glm.hpp>
#include "shader.h"

class Light {
protected:
    glm::vec3 position_;

    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;

public:

    Light(glm::vec3 position, glm::vec3 ambient = glm::vec3(1.f),
          glm::vec3 diffuse = glm::vec3(.2f), glm::vec3 specular = glm::vec3(.5f));

    virtual void set_direction(glm::vec3);

    void set_position(glm::vec3 position);

    virtual void attach_to_shader(Shader *shader) = 0;
};