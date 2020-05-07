#pragma once

#include <iostream>
#include <stdexcept>

#ifndef __glew_h__

#include <GL/glew.h>

#endif

#include <GLFW/glfw3.h>

class Window {
private:
    int width_{1000};
    int height_{800};

    GLFWwindow *window_;

private:
    static void handle_errors_(int error, const char *description);

public:

    Window();

    ~Window();

    inline GLFWwindow *get_window() const { return window_; }

    inline int get_width() { return width_; }

    inline int get_height() { return height_; }

    void set_title(const std::string &title);
};
