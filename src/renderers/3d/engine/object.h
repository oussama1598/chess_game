#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"

#include "obj_loader.h"

class Object {
private:
    Mesh *mesh_;
    Texture *texture_diffuse_;
    Texture *texture_specular_;
    Material *material_;
    glm::mat4 model_matrix_{1.f};

private:

    void calculate_model_matrix_();

public:
    glm::vec3 position_{0.f};
    glm::vec3 rotation_{0.f};
    glm::vec3 scale_{1.f};

public:
    Object(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices);

    Object(const std::string &file_path);

    ~Object();

    void draw(Shader &shader);
};
