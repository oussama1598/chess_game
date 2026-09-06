#include "texture.h"

#include <algorithm>

Texture::Texture(const std::string &file_path) {
    int image_width = 0;
    int image_height = 0;
    unsigned char *image = stbi_load(
            file_path.c_str(),
            &image_width,
            &image_height,
            nullptr,
            STBI_rgb_alpha
    );

    if (!image)
        throw std::runtime_error(
                "Texture loading failed for " + file_path + ": " +
                (stbi_failure_reason() == nullptr ? "unknown error" : stbi_failure_reason()));

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if (GLEW_EXT_texture_filter_anisotropic) {
        GLfloat maximum_anisotropy = 1.f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximum_anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT,
                        std::min(maximum_anisotropy, 8.f));
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);
    unbind();
}

Texture::~Texture() {
    glDeleteTextures(1, &texture_id_);
}

void Texture::bind(int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
