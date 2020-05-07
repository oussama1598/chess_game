#pragma once

#include "object.h"

class SkyBox : public Object {
private:
    GLuint texture_id_;
    Mesh *mesh_cube_;

    // TODO: has to change
    std::map<GLuint, std::string> faces_textures_{
            {GL_TEXTURE_CUBE_MAP_POSITIVE_X, "./assets/sprites/scene_right.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "./assets/sprites/scene_left.jpg"},
            {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "./assets/sprites/scene_top.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "./assets/sprites/scene_bottom.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "./assets/sprites/scene_back.jpg"},
            {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "./assets/sprites/scene_front.jpg"}
    };

private:
    void set_texture_face_(GLuint type, const std::string &file_path);

    void bind_texture_();

    void unbind_texture_();

public:

    SkyBox();

    ~SkyBox();

    void attach_to_shader(Shader *shader);

    void draw() override;
};
