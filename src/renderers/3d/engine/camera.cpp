#include "camera.h"

Camera::Camera(const glm::vec3 &position, const glm::vec2 &size) : position_{position} {
    world_up_ = glm::vec3{0.f, 1.f, 0.f};
    up_ = glm::vec3{0.f, 1.f, 0.f};
    right_ = glm::vec3(0.f);
    view_matrix_ = glm::mat4(1.f);
    rotation_ = glm::vec3{0.f, -90.f, 0.f};

    // initialize the projection matrix
    projection_matrix_ = glm::perspective(
            glm::radians(field_of_view_),
            (float) (size.x / size.y),
            near_plane_,
            far_plane_
    );

    recalculate_view_matrix_();
}

void Camera::recalculate_view_matrix_() {
    front_.x = cos(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
    front_.y = sin(glm::radians(rotation_.x));
    front_.z = sin(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));

    front_ = glm::normalize(front_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_ = glm::normalize(glm::cross(right_, front_));

    view_matrix_ = glm::lookAt(position_, position_ + front_, up_);
}

void Camera::move(const float &dt, Movement_Direction direction, GLfloat amount) {
    //Update position vector
    switch (direction) {
        case Movement_Direction::FORWARD:
            position_ += front_ * amount * dt;
            break;
        case Movement_Direction::BACKWARD:
            position_ -= front_ * amount * dt;
            break;
        case Movement_Direction::LEFT:
            position_ -= right_ * amount * dt;
            break;
        case Movement_Direction::RIGHT:
            position_ += right_ * amount * dt;
            break;
        default:
            break;
    }

    recalculate_view_matrix_();
}

void Camera::rotate(float dt, double offset_x, double offset_y, float sensitivity) {
    rotation_.x -= static_cast<GLfloat>(offset_y) * sensitivity * dt;
    rotation_.y += static_cast<GLfloat>(offset_x) * sensitivity * dt;

    if (rotation_.x > 80.f)
        rotation_.x = 80.f;
    else if (rotation_.x < -80.f)
        rotation_.x = -80.f;

    if (rotation_.y > 360.f || rotation_.y < -360.f)
        rotation_.y = 0.f;

    recalculate_view_matrix_();
}

void Camera::attach_to_shader(Shader *shader) {
    shader->bind();

    shader->set_uniform_matrix_4_fv("view_matrix", get_view_matrix());
    shader->set_uniform_matrix_4_fv("projection_matrix", get_projection_matrix());
}
