#pragma once

#include <map>
#include <string>

#include "object.h"

class SkyBox : public Object {
private:
    GLuint texture_id_;
    Mesh *mesh_cube_;

    std::map<GLuint, std::string> faces_textures_{
            {GL_TEXTURE_CUBE_MAP_POSITIVE_X, "./assets/textures/combination_room/right.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "./assets/textures/combination_room/left.jpg"},
            {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "./assets/textures/combination_room/top.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "./assets/textures/combination_room/bottom.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "./assets/textures/combination_room/back.jpg"},
            {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "./assets/textures/combination_room/front.jpg"}
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
