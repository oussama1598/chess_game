#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class Camera {
private:
    glm::vec3 position_;
    glm::vec3 rotation_;

    glm::vec3 front_;
    glm::vec3 right_;

    glm::vec3 world_up_;
    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;

    float field_of_view_{90.f};
    float near_plane_{0.1f};
    float far_plane_{1000.f};

private:
    void recalculate_view_matrix_();

public:
    enum Movement_Direction {
        FORWARD = 0, BACKWARD, LEFT, RIGHT
    };

public:
    Camera(const glm::vec3 &position, const glm::vec2 &size);

    [[nodiscard]] inline glm::vec3 get_position() const { return position_ + front_; }

    inline glm::vec3 get_rotation() const { return rotation_; }

    [[nodiscard]] inline glm::mat4 get_view_matrix() const { return view_matrix_; }

    inline glm::mat4 get_projection_matrix() const { return projection_matrix_; }

    void move(const float &dt, Movement_Direction direction, GLfloat amount);

    void rotate(float dt, double offset_x, double offset_y, float sensitivity);

    void attach_to_shader(Shader *shader);
};
