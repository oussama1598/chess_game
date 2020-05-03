#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
    GLuint program_id_;

private:

    static std::string load_from_file_(const std::string &file_path);

    static GLuint compiler_shader_(unsigned int type, const std::string &file_path);

public:

    Shader();

    ~Shader();

    void bind() const;
    void unbind() const;

    void set_uniform_matrix_4_fv(const std::string &name, const glm::mat4 &matrix) const;

    void set_uniform_1_i(const std::string &name, GLint value) const;

    void set_uniform_1_f(const std::string &name, const GLfloat value) const;

    void set_uniform_3_fv(const std::string &name, const glm::vec3 &vector) const;
};
