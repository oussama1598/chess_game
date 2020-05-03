#include "object.h"

Object::Object(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices) {
    mesh_ = new Mesh(vertices, indices);
    texture_diffuse_ = new Texture(
            "/home/red-scule/Desktop/projects/cpp_projects/opengl_tuto/src/images/container.png");
    texture_specular_ = new Texture(
            "/home/red-scule/Desktop/projects/cpp_projects/opengl_tuto/src/images/container_specular.png");
    material_ = new Material(
            glm::vec3(0.1f),
            glm::vec3(1.f),
            glm::vec3(2.f),
            0,
            1
    );

    calculate_model_matrix_();
}

Object::Object(const std::string &file_path) : Object(Obj_Loader::load_obj(file_path), {}) {}

Object::~Object() {
    delete texture_diffuse_;
    delete texture_specular_;
    delete material_;
    delete mesh_;
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

void Object::draw(Shader &shader) {
    calculate_model_matrix_();

    texture_diffuse_->bind(0);
    texture_specular_->bind(1);
    material_->assign_to_shader(shader);

    shader.set_uniform_matrix_4_fv("model_matrix", model_matrix_);

    mesh_->render();
}
