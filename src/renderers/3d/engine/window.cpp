#include "window.h"

#include <algorithm>

Window::Window() {
    glfwSetErrorCallback(handle_errors_);

#if defined(__linux__)
    // GLEW uses GLX on Linux, so prefer X11 (or XWayland) over GLFW's
    // automatic Wayland selection.
    if (glfwPlatformSupported(GLFW_PLATFORM_X11)) {
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
    }
#endif

    if (!glfwInit())
        throw std::runtime_error("Couldn't initialize glfw.");

    // configure glfw to use the OPENGL version 4.4 with the core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window_ = glfwCreateWindow(width_, height_, "Chess 3D", nullptr, nullptr);

    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Couldn't create a window.");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;

    const GLenum glew_status = glewInit();
    const bool core_functions_are_available =
            glew_status == GLEW_ERROR_NO_GLX_DISPLAY && GLEW_VERSION_4_4;
    if (glew_status != GLEW_OK && !core_functions_are_available) {
        const std::string error_message = reinterpret_cast<const char *>(
                glewGetErrorString(glew_status));
        glfwDestroyWindow(window_);
        window_ = nullptr;
        glfwTerminate();
        throw std::runtime_error("Couldn't initialize GLEW: " + error_message);
    }

    // GLEW may probe an unavailable legacy extension in a core profile.
    // Clear that harmless error so it does not leak into renderer diagnostics.
    while (glGetError() != GL_NO_ERROR) {}

    if (const auto *renderer = glGetString(GL_RENDERER)) {
        gpu_name_ = reinterpret_cast<const char *>(renderer);
    }

    if (GLFWmonitor *monitor = glfwGetPrimaryMonitor()) {
        if (const GLFWvidmode *mode = glfwGetVideoMode(monitor)) {
            constexpr int side_panel_width = 262;
            const int x = std::max(0, (mode->width - width_ - side_panel_width) / 2);
            const int y = std::max(0, (mode->height - height_) / 2);
            glfwSetWindowPos(window_, x, y);
        }
    }

    glfwShowWindow(window_);
    glfwFocusWindow(window_);

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << " | " << glGetString(GL_VENDOR)
              << " | " << gpu_name_ << std::endl;
}

Window::~Window() {
    if (window_ != nullptr) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

void Window::handle_errors_([[maybe_unused]] int error, const char *description) {
    std::cout << "Error" << " " << description << std::endl;
}

void Window::set_title(const std::string &title) {
    glfwSetWindowTitle(window_, title.c_str());
}
