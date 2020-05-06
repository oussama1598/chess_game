#include "object.h"

Object::Object() {
    calculate_model_matrix_();
}

void Object::calculate_model_matrix_() {
    model_matrix_ = glm::mat4{1.f};
    model_matrix_ = glm::translate(model_matrix_, position_);
    // rotate along x
    model_matrix_ = glm::rotate(model_matrix_, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));
    // rotate along y
    model_matrix_ = glm::rotate(model_matrix_, glm::radians(rotation_.y), glm::vec3(0.f, 1.f, 0.f));
    // rotate along z
    model_matrix_ = glm::rotate(model_matrix_, glm::radians(rotation_.z), glm::vec3(0.f, 0.f, 1.f));
    model_matrix_ = glm::scale(model_matrix_, scale_);

}

void Object::set_mesh(Mesh *mesh) {
    mesh_ = mesh;
}

void Object::set_shader(Shader *shader) {
    shader_ = shader;
}

void Object::set_diffuse_texture(Texture *texture) {
    texture_diffuse_ = texture;
}

void Object::set_specular_texture(Texture *texture) {
    texture_specular_ = texture;
}

void Object::set_material(Material *material) {
    material_ = material;
}

void Object::translate(glm::vec3 translate_vector) {
    position_ += translate_vector;
}

void Object::move_to(glm::vec3 position)
{
    position_ = position;
}

void Object::draw() {
    calculate_model_matrix_();

    shader_->set_uniform_1_i("material.use_texture", 0);

    if (texture_diffuse_ != nullptr) {
        texture_diffuse_->bind(0);

        shader_->set_uniform_1_i("material.use_texture", 1);
    }

    if (texture_specular_ != nullptr) texture_specular_->bind(1);

    material_->assign_to_shader(*shader_);

    shader_->set_uniform_matrix_4_fv("model_matrix", model_matrix_);

    shader_->bind();
    mesh_->render();
    shader_->unbind();
}
