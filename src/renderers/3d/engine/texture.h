#pragma once

#include <stdexcept>
#include <GL/glew.h>
#include <stb_image.h>

class Texture {
private:
    GLuint texture_id_{};

public:
    Texture(const std::string &file_path);

    ~Texture();

    void bind(int slot);

    void unbind();
};
