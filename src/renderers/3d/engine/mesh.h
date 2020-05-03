#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh {

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 text_coord;
        glm::vec3 normal;
    };

private:
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;

    GLuint VAO_; // vertex array object
    GLuint VBO_; // vertex buffer object for vertices
    GLuint EBO_; // vertex elements object for indices

public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices);

    ~Mesh();

    void render();

private:
    void init_vertex_array_();

    void init_vertex_buffer_();

    void init_vertex_elements_();

    void init_attributes_();
};
