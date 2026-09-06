#include "obj_loader.h"

#include <stdexcept>

namespace {
int resolve_index(const std::string &value, size_t size) {
    if (value.empty()) {
        return -1;
    }

    const int parsed = std::stoi(value);
    const int resolved = parsed > 0 ? parsed - 1 : static_cast<int>(size) + parsed;
    if (resolved < 0 || resolved >= static_cast<int>(size)) {
        throw std::runtime_error("OBJ index is out of range");
    }
    return resolved;
}
}

Obj_Loader::Obj_Data Obj_Loader::load_file_(const std::string &file_path) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> text_coordinates;
    std::vector<glm::vec3> normals;
    std::vector<std::string> faces;

    std::string temp;

    std::ifstream file(file_path);

    if (!file.is_open())
        throw std::runtime_error("Couldn't open file " + file_path);

    while (getline(file, temp)) {
        std::stringstream temp_stream;
        std::string prefix;

        temp_stream << temp;
        temp_stream >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;

            temp_stream >> vertex.x >> vertex.y >> vertex.z;

            positions.push_back(vertex);
        }

        if (prefix == "vt") {
            glm::vec2 texture_coordinates;

            temp_stream >> texture_coordinates.x >> texture_coordinates.y;

            text_coordinates.push_back(texture_coordinates);
        }

        if (prefix == "vn") {
            glm::vec3 normal;

            temp_stream >> normal.x >> normal.y >> normal.z;

            normals.push_back(normal);
        }

        if (prefix == "f") {
            std::string face;
            std::getline(temp_stream >> std::ws, face);
            faces.push_back(face);
        }
    }


    return {positions, text_coordinates, normals, faces};
}

std::vector<Vertex> Obj_Loader::load_obj(const std::string &file_path) {
    Obj_Data object_data = load_file_(file_path);
    std::vector<Vertex> vertices;

    for (auto &face: object_data.faces) {
        std::istringstream face_stream(face);
        std::string vertex_description;
        std::vector<Vertex> face_vertices;

        while (face_stream >> vertex_description) {
            std::array<std::string, 3> indices{};
            std::istringstream index_stream(vertex_description);
            for (std::string &index: indices) {
                if (!std::getline(index_stream, index, '/')) {
                    index.clear();
                }
            }

            const int position_index = resolve_index(indices[0], object_data.positions.size());
            const int texture_index = resolve_index(indices[1], object_data.text_coordinates.size());
            const int normal_index = resolve_index(indices[2], object_data.normals.size());

            if (position_index == -1) {
                throw std::runtime_error("OBJ face is missing a position index");
            }
            glm::vec2 texture_coordinate{0.f};
            glm::vec3 normal{0.f, 1.f, 0.f};

            if (texture_index != -1)
                texture_coordinate = object_data.text_coordinates[texture_index];
            if (normal_index != -1)
                normal = object_data.normals[normal_index];

            face_vertices.push_back(
                    {
                            object_data.positions[position_index],
                            glm::vec3{1.f, 1.f, 1.f},
                            texture_coordinate,
                            normal,
                    }
            );
        }

        if (face_vertices.size() < 3) {
            throw std::runtime_error("OBJ face has fewer than three vertices");
        }

        for (size_t i = 1; i + 1 < face_vertices.size(); ++i) {
            vertices.push_back(face_vertices[0]);
            vertices.push_back(face_vertices[i]);
            vertices.push_back(face_vertices[i + 1]);
        }
    }

    return vertices;
}
