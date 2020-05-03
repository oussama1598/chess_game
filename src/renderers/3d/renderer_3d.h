#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "engine/shader.h"
#include "engine/camera.h"
#include "engine/light.h"
#include "primitives/triangle.h"
#include "primitives/plane.h"
#include "primitives/pyramid.h"

class GL_Window {
private:
    int width_{800};
    int height_{800};

    GLFWwindow *window_;

    Camera *camera;
    Light *light;
    Shader *main_shader_;

    Object *object_;

    float dt_{0};
    float current_time_{0};
    float last_time_{0};

    double last_mouse_x_{0};
    double last_mouse_y_{0};
    double mouse_x_{0};
    double mouse_y_{0};
    double mouse_offset_x_{0};
    double mouse_offset_y_{0};
    bool first_move_{true};

private:
    static void handle_errors_(int error, const char *description);

    void init_gl_();

    void update_dt_();

    void update_shaders_();

public:

    GL_Window();

    ~GL_Window();

    void update_mouse_input();

    void update_keyboard_input();

    void render();
};
