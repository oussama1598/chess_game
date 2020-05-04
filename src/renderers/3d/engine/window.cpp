#include "window.h"

Window::Window() {
    glfwSetErrorCallback(handle_errors_);

    if (!glfwInit())
        std::runtime_error("Couldn't initialize glfw.");

    // configure glfw to use the OPENGL version 4.4 with the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window_ = glfwCreateWindow(width_, height_, "Chess Game", nullptr, nullptr);

    if (!window_) {
        std::runtime_error("Couldn't create a window.");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::runtime_error("Couldn't initialize glew.");
    }

    std::cout << "OpenGL version supported by this platform: " << glGetString(GL_VERSION)
              << std::endl;
}

Window::~Window() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Window::handle_errors_([[maybe_unused]] int error, const char *description) {
    std::cout << "Error" << " " << description << std::endl;
}

void Window::set_title(const std::string &title) {
    glfwSetWindowTitle(window_, title.c_str());
}
