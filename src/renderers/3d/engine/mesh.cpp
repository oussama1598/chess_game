#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices) :
        vertices_{vertices}, indices_{indices} {
    init_vertex_array_();
    init_vertex_buffer_();
    init_vertex_elements_();
    init_attributes_();

    glBindVertexArray(0);
}

Mesh::Mesh(const std::string &file_path) : Mesh(Obj_Loader::load_obj(file_path), {}) {}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}

void Mesh::init_vertex_array_() {
    glCreateVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);
}

void Mesh::init_vertex_buffer_() {
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), vertices_.data(),
                 GL_STATIC_DRAW);
}

void Mesh::init_vertex_elements_() {
    glGenBuffers(1, &EBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices_.size(), indices_.data(),
                 GL_STATIC_DRAW);
}

void Mesh::init_attributes_() {
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, text_coord));
    glEnableVertexAttribArray(2);

    // normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, normal));
    glEnableVertexAttribArray(3);
}

void Mesh::render() {
    glBindVertexArray(VAO_);

    if (indices_.empty())
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    else
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
