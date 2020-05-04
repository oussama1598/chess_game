#pragma once

#include "object.h"

class SkyBox : public Object {
private:
    GLuint texture_id_;

    // TODO: has to change
    std::map<GLuint, std::string> faces_textures_{
            {GL_TEXTURE_CUBE_MAP_POSITIVE_X, "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/comawhite_right.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/comawhite_left.jpg"},
            {GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/comawhite_top.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/comawhite_bottom.jpg"},
            {GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/comawhite_back.jpg"},
            {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "/home/red-scule/Desktop/projects/cpp_projects/chess_game/assets/sprites/comawhite_front.jpg"}
    };

private:
    void set_texture_face_(GLuint type, const std::string &file_path);

    void bind_texture_();

    void unbind_texture_();

public:

    SkyBox();

    void attach_to_shader(Shader *shader);

    void draw() override;
};
