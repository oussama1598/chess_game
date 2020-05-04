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
    Mesh *mesh_{nullptr};
    Shader *shader_{nullptr};
    Texture *texture_diffuse_{nullptr};
    Texture *texture_specular_{nullptr};
    Material *material_{nullptr};

    glm::mat4 model_matrix_{1.f};

private:

    void calculate_model_matrix_();

public:
    glm::vec3 position_{0.f};
    glm::vec3 rotation_{0.f};
    glm::vec3 scale_{1.f};

public:
    Object();

    inline Shader *get_shader() const { return shader_; }

    void set_mesh(Mesh *mesh);

    void set_shader(Shader *shader);

    void set_diffuse_texture(Texture *texture);

    void set_specular_texture(Texture *texture);

    void set_material(Material *material);

    void draw();

    void translate(glm::vec3 translate_vector);
};
