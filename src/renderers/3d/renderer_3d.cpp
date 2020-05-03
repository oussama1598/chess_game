#include "gl_window.h"

// create material
// new Material()
// material.add_texture(texture);
// material.add_shader(shader)
// object.add_material(material)
// create a scene
// scene.add_object(object)
// scene.add_camera(camera) this should make sure that all shaders have the projection and view matrix
// scene.add_light(light);

GL_Window::GL_Window() {
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

    init_gl_();

    while (!glfwWindowShouldClose(window_)) {
        update_dt_();

        // update shaders
        update_shaders_();

        // update inputs
        glfwPollEvents();

        // update inputs
        update_mouse_input();
        update_keyboard_input();

        // draw
        render();
    }
}

GL_Window::~GL_Window() {
    delete main_shader_;
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void GL_Window::handle_errors_([[maybe_unused]] int error, const char *description) {
    std::cout << "Error" << " " << description << std::endl;
}

void GL_Window::init_gl_() {
    // enable z axe
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // fill shape with color, GL_LINES will only draw the wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // for the alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // init camera
    camera = new Camera(
            {0.f, 0.f, 2.f},
            {width_, height_}
    );
    light = new Light(
            {0.f, 0.f, 2.f},
            {1.f, 0.f, 0.f},
            1
    );

    // init shaders
    main_shader_ = new Shader();
    object_ = new Object(
            "/home/red-scule/Desktop/projects/cpp_projects/opengl_tuto/src/models/king.obj"
    );
}

void GL_Window::update_dt_() {
    current_time_ = (float) glfwGetTime();
    dt_ = current_time_ - last_time_;

    last_time_ = current_time_;
}

void GL_Window::update_shaders_() {
    light->attach_to_shader(main_shader_);

    main_shader_->bind();
    main_shader_->set_uniform_matrix_4_fv("view_matrix", camera->get_view_matrix());
    main_shader_->set_uniform_matrix_4_fv("projection_matrix", camera->get_projection_matrix());

    // lights
    main_shader_->set_uniform_3_fv("camera_position", camera->get_position());

    main_shader_->unbind();
}

void GL_Window::update_mouse_input() {
    glfwGetCursorPos(window_, &mouse_x_, &mouse_y_);

    if (first_move_) {
        last_mouse_x_ = mouse_x_;
        last_mouse_y_ = mouse_y_;
        first_move_ = false;
    }

    mouse_offset_x_ = mouse_x_ - last_mouse_x_;
    mouse_offset_y_ = mouse_y_ - last_mouse_y_;

    last_mouse_x_ = mouse_x_;
    last_mouse_y_ = mouse_y_;

    camera->rotate(dt_, mouse_offset_x_, mouse_offset_y_, 5.f);
}


void GL_Window::update_keyboard_input() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        camera->move(dt_, Camera::Movement_Direction::FORWARD, 1);
    }

    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        camera->move(dt_, Camera::Movement_Direction::BACKWARD, 1);
    }

    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        camera->move(dt_, Camera::Movement_Direction::RIGHT, 1);
    }

    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        camera->move(dt_, Camera::Movement_Direction::LEFT, 1);
    }

    if (glfwGetKey(window_, GLFW_KEY_L) == GLFW_PRESS) {
        light->set_position(camera->get_position());
    }
}

void GL_Window::render() {
    glClearColor(0.f, 0.f, 0.f, 1.f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the core program
    main_shader_->bind();

    object_->draw(*main_shader_);

    glfwSwapBuffers(window_);
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
