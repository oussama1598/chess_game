#include "camera.h"

#include <algorithm>
#include <cmath>

Camera::Camera(const glm::vec3 &position, const glm::vec2 &size) : position_{position} {
    projection_matrix_ = glm::perspective(
            glm::radians(field_of_view_),
            size.x / size.y,
            near_plane_,
            far_plane_);

    sync_orbit_from_position_();
    recalculate_view_matrix_();
}

void Camera::sync_orbit_from_position_() {
    const glm::vec3 offset = position_ - target_;
    orbit_distance_ = std::max(glm::length(offset), 0.001f);
    orbit_pitch_ = glm::degrees(std::asin(offset.y / orbit_distance_));
    orbit_yaw_ = glm::degrees(std::atan2(offset.z, offset.x));
}

void Camera::recalculate_orbit_position_() {
    const float pitch = glm::radians(orbit_pitch_ + selection_pitch_offset_);
    const float yaw = glm::radians(orbit_yaw_);
    const float horizontal_distance = orbit_distance_ * std::cos(pitch);

    position_.x = target_.x + horizontal_distance * std::cos(yaw);
    position_.y = target_.y + orbit_distance_ * std::sin(pitch);
    position_.z = target_.z + horizontal_distance * std::sin(yaw);
    recalculate_view_matrix_();
}

void Camera::recalculate_view_matrix_() {
    front_ = glm::normalize(target_ - position_);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    view_matrix_ = glm::lookAt(position_, target_, world_up_);
}

void Camera::set_orbit(const glm::vec3 &position, const glm::vec3 &target) {
    position_ = position;
    target_ = target;
    selection_pitch_offset_ = 0.f;
    selection_pitch_target_ = 0.f;
    selection_transition_active_ = false;
    sync_orbit_from_position_();
    recalculate_view_matrix_();
}

void Camera::move(float dt, Movement_Direction direction, GLfloat amount) {
    cancel_selection_transition_();
    switch (direction) {
        case FORWARD:
            orbit_distance_ = std::max(5.5f, orbit_distance_ - amount * dt * 3.f);
            break;
        case BACKWARD:
            orbit_distance_ = std::min(15.f, orbit_distance_ + amount * dt * 3.f);
            break;
        case LEFT:
            orbit_yaw_ += amount * dt * 45.f;
            break;
        case RIGHT:
            orbit_yaw_ -= amount * dt * 45.f;
            break;
    }
    recalculate_orbit_position_();
}

void Camera::orbit(double offset_x, double offset_y, float sensitivity) {
    cancel_selection_transition_();
    orbit_yaw_ -= static_cast<float>(offset_x) * sensitivity;
    orbit_pitch_ -= static_cast<float>(offset_y) * sensitivity;
    orbit_pitch_ = std::clamp(orbit_pitch_, 18.f, 78.f);
    recalculate_orbit_position_();
}

void Camera::zoom(double amount) {
    cancel_selection_transition_();
    orbit_distance_ = std::clamp(
            orbit_distance_ - static_cast<float>(amount) * .65f,
            5.5f,
            15.f);
    recalculate_orbit_position_();
}

void Camera::set_selection_focus(bool active) {
    const float target_offset = active ? 6.f : 0.f;
    if (std::abs(selection_pitch_target_ - target_offset) < .001f &&
        (selection_transition_active_ ||
         std::abs(selection_pitch_offset_ - target_offset) < .001f)) {
        return;
    }

    selection_pitch_start_ = selection_pitch_offset_;
    selection_pitch_target_ = target_offset;
    selection_transition_time_ = 0.f;
    selection_transition_duration_ = active ? .22f : .26f;
    selection_transition_active_ = true;
}

void Camera::update(float dt) {
    if (!selection_transition_active_) return;

    selection_transition_time_ += dt;
    const float progress = std::clamp(
            selection_transition_time_ / selection_transition_duration_, 0.f, 1.f);
    const float eased = progress * progress * (3.f - 2.f * progress);
    selection_pitch_offset_ = glm::mix(selection_pitch_start_, selection_pitch_target_, eased);
    if (progress >= 1.f) {
        selection_pitch_offset_ = selection_pitch_target_;
        selection_transition_active_ = false;
    }
    recalculate_orbit_position_();
}

void Camera::cancel_selection_transition_() {
    if (std::abs(selection_pitch_offset_) > .001f) {
        orbit_pitch_ = std::clamp(orbit_pitch_ + selection_pitch_offset_, 18.f, 78.f);
    }
    selection_pitch_offset_ = 0.f;
    selection_pitch_start_ = 0.f;
    selection_pitch_target_ = 0.f;
    selection_transition_active_ = false;
}

void Camera::attach_to_shader(Shader *shader) {
    shader->set_uniform_matrix_4_fv("view_matrix", view_matrix_);
    shader->set_uniform_matrix_4_fv("projection_matrix", projection_matrix_);
    shader->set_uniform_3_fv("camera_position", position_);
}

void Camera::attach_skybox_to_shader(Shader *shader) {
    // A skybox follows the camera position while preserving its rotation.
    // Removing translation prevents parallax and keeps it inside the far plane.
    shader->set_uniform_matrix_4_fv("view_matrix", glm::mat4(glm::mat3(view_matrix_)));
    shader->set_uniform_matrix_4_fv("projection_matrix", projection_matrix_);
    shader->set_uniform_3_fv("camera_position", position_);
}
