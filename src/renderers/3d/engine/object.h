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
protected:
    Mesh *mesh_{nullptr};
    Shader *shader_{nullptr};
    Texture *texture_diffuse_{nullptr};
    Texture *texture_specular_{nullptr};
    Material *material_{nullptr};
    bool selectable_{true};
    bool casts_shadow_{true};
    bool chess_board_{false};
    float surface_seed_{0.f};
    glm::vec2 texture_scale_{1.f};

    glm::mat4 model_matrix_{1.f};

protected:

    void calculate_model_matrix_();

public:
    glm::vec3 position_{0.f};
    glm::vec3 rotation_{0.f};
    glm::vec3 scale_{1.f};

public:
    Object();

    virtual ~Object() = default;

    virtual void set_mesh(Mesh *mesh);

    virtual void set_shader(Shader *shader);

    inline Shader *get_shader() const { return shader_; }

    virtual void set_diffuse_texture(Texture *texture);

    virtual void set_specular_texture(Texture *texture);

    virtual void set_material(Material *material);

    virtual Material *get_material() const { return material_; }

    inline void set_selectable(bool selectable) { selectable_ = selectable; }

    [[nodiscard]] inline bool is_selectable() const { return selectable_; }

    inline void set_casts_shadow(bool casts_shadow) { casts_shadow_ = casts_shadow; }

    [[nodiscard]] inline bool casts_shadow() const { return casts_shadow_; }

    inline void set_chess_board(bool chess_board) { chess_board_ = chess_board; }

    inline void set_surface_seed(float surface_seed) { surface_seed_ = surface_seed; }

    inline void set_texture_scale(glm::vec2 texture_scale) { texture_scale_ = texture_scale; }

    virtual void draw();

    glm::vec3 &get_position() { return position_; }

    virtual void move_to(glm::vec3 position);

    virtual void translate(glm::vec3 translate_vector);

    virtual void rotate(glm::vec3 rotation_vector);
};
