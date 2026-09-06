#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class Camera {
private:
    glm::vec3 position_{};
    glm::vec3 target_{0.f, .30f, 0.f};
    glm::vec3 front_{};
    glm::vec3 right_{};
    glm::vec3 world_up_{0.f, 1.f, 0.f};
    glm::mat4 view_matrix_{1.f};
    glm::mat4 projection_matrix_{1.f};

    float orbit_yaw_{90.f};
    float orbit_pitch_{40.f};
    float orbit_distance_{9.5f};
    float selection_pitch_offset_{0.f};
    float selection_pitch_start_{0.f};
    float selection_pitch_target_{0.f};
    float selection_transition_time_{0.f};
    float selection_transition_duration_{0.f};
    bool selection_transition_active_{false};

    float field_of_view_{50.f};
    float near_plane_{0.1f};
    float far_plane_{100.f};

private:
    void sync_orbit_from_position_();
    void recalculate_orbit_position_();
    void recalculate_view_matrix_();
    void cancel_selection_transition_();

public:
    enum Movement_Direction {
        FORWARD = 0, BACKWARD, LEFT, RIGHT
    };

    Camera(const glm::vec3 &position, const glm::vec2 &size);

    [[nodiscard]] inline glm::vec3 get_position() const { return position_; }
    [[nodiscard]] inline glm::mat4 get_view_matrix() const { return view_matrix_; }
    [[nodiscard]] inline glm::mat4 get_projection_matrix() const { return projection_matrix_; }

    void set_orbit(const glm::vec3 &position, const glm::vec3 &target);
    void move(float dt, Movement_Direction direction, GLfloat amount);
    void orbit(double offset_x, double offset_y, float sensitivity = .16f);
    void zoom(double amount);
    void set_selection_focus(bool active);
    void update(float dt);
    void attach_to_shader(Shader *shader);
    void attach_skybox_to_shader(Shader *shader);
};
