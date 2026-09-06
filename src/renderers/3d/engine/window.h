#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

#ifndef __glew_h__

#include <GL/glew.h>

#endif

#include <GLFW/glfw3.h>

class Window {
private:
    int width_{1000};
    int height_{800};

    GLFWwindow *window_{nullptr};
    std::string gpu_name_{"Unknown GPU"};

private:
    static void handle_errors_(int error, const char *description);

public:

    Window();

    ~Window();

    inline GLFWwindow *get_window() const { return window_; }

    [[nodiscard]] inline int get_width() const { return width_; }

    [[nodiscard]] inline int get_height() const { return height_; }

    [[nodiscard]] inline const std::string &get_gpu_name() const { return gpu_name_; }

    void set_title(const std::string &title);
};
