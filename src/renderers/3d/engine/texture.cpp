#include "texture.h"

Texture::Texture(const std::string &file_path) {
    int image_width = 0;
    int image_height = 0;
    unsigned char *image = SOIL_load_image(
            file_path.c_str(),
            &image_width,
            &image_height,
            nullptr,
            SOIL_LOAD_RGBA
    );

    if (!image)
        throw std::runtime_error("Texture loading failed");

    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_width, image_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
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
    glActiveTexture(0);
}
