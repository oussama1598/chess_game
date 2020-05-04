#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

#include "obj_loader.h"

class Mesh {

private:
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;

    GLuint VAO_; // vertex array object
    GLuint VBO_; // vertex buffer object for vertices
    GLuint EBO_; // vertex elements object for indices

public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

    Mesh(const std::string &file_path);

    ~Mesh();

    void render();

private:
    void init_vertex_array_();

    void init_vertex_buffer_();

    void init_vertex_elements_();

    void init_attributes_();
};
