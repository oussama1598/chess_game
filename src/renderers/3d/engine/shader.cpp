#include "shader.h"

Shader::Shader(const std::string &vertex_file_path, const std::string &fragment_file_path) {
    program_id_ = glCreateProgram();

    GLuint vertex_shader = compiler_shader_(
            GL_VERTEX_SHADER,
            vertex_file_path
    );

    GLuint fragment_shader = compiler_shader_(
            GL_FRAGMENT_SHADER,
            fragment_file_path
    );

    glAttachShader(program_id_, vertex_shader);
    glAttachShader(program_id_, fragment_shader);
    glLinkProgram(program_id_);

    GLint state{0};

    glGetProgramiv(program_id_, GL_LINK_STATUS, &state);

    if (!state) {
        int length;
        glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &length);

        char *info_log = new char[length];

        glGetProgramInfoLog(program_id_, length, nullptr, info_log);

        throw std::runtime_error(
                "Linking error for \n reason" + std::string(info_log));
    }

    glUseProgram(0); // revert any setting of the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() {
    glDeleteProgram(program_id_);
}

std::string Shader::load_from_file_(const std::string &file_path) {
    std::ifstream file{file_path};
    std::string temp;
    std::stringstream source;

    if (!file.is_open())
        throw std::runtime_error("Could not load file " + file_path);

    while (getline(file, temp)) {
        source << temp << "\n";
    }

    file.close();

    return source.str();
}

GLuint Shader::compiler_shader_(unsigned int type, const std::string &file_path) {
    GLint state{0};
    GLuint shader_id = glCreateShader(type);
    std::string source = load_from_file_(file_path);
    const GLchar *source_c_str = source.c_str();

    glShaderSource(shader_id, 1, &source_c_str, nullptr);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &state);

    if (!state) {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

        char *info_log = new char[length];

        glGetShaderInfoLog(shader_id, length, nullptr, info_log);

        throw std::runtime_error(
                "Compilation error for " + file_path + "\n reason" + std::string(info_log));
    }

    return shader_id;
}

void Shader::bind() const {
    glUseProgram(program_id_);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::set_uniform_matrix_4_fv(const std::string &name, const glm::mat4 &matrix) const {
    bind();
    glUniformMatrix4fv(
            glGetUniformLocation(program_id_, name.c_str()),
            1,
            GL_FALSE,
            glm::value_ptr(matrix)
    );
    unbind();
}

void Shader::set_uniform_1_i(const std::string &name, const GLint value) const {
    bind();
    glUniform1i(
            glGetUniformLocation(program_id_, name.c_str()),
            value
    );
    unbind();
}

void Shader::set_uniform_1_f(const std::string &name, const GLfloat value) const {
    bind();
    glUniform1f(
            glGetUniformLocation(program_id_, name.c_str()),
            value
    );
    unbind();
}

void Shader::set_uniform_3_fv(const std::string &name, const glm::vec3 &vector) const {
    bind();
    glUniform3fv(
            glGetUniformLocation(program_id_, name.c_str()),
            1,
            glm::value_ptr(vector)
    );
    unbind();
}
