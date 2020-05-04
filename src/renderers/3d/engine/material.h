#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Material {

private:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
    float shininess_;
    GLint diffuse_texture_;
    GLint specular_texture_;

public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
             float shininess,
             GLint diffuse_texture,
             GLint specular_texture);

    void assign_to_shader(Shader &shader);
};
