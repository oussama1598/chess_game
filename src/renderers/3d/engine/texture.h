#pragma once

#include <stdexcept>
#include <GL/glew.h>
#include <SOIL2/SOIL2.h>

class Texture {
private:
    GLuint texture_id_{};

public:
    Texture(const std::string &file_path);

    ~Texture();

    void bind(int slot);

    void unbind();
};
