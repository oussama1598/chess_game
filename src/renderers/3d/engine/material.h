#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Material {

private:
    glm::vec3 base_color_;
    float roughness_;
    float metallic_;
    float ao_;
    float clearcoat_;
    GLint surface_style_;
    float albedo_variation_;
    float roughness_variation_;
    float micro_normal_strength_;
    float direct_specular_scale_;
    float environment_specular_scale_;
    GLint diffuse_texture_;
    GLint specular_texture_;

public:
    Material(glm::vec3 base_color, float roughness, float metallic,
             float ao, float clearcoat,
             GLint diffuse_texture,
             GLint specular_texture,
             GLint surface_style = 0,
             float albedo_variation = 0.f,
             float roughness_variation = 0.f,
             float micro_normal_strength = 0.f,
             float direct_specular_scale = 1.f,
             float environment_specular_scale = 1.f);

    void assign_to_shader(Shader &shader);
};
