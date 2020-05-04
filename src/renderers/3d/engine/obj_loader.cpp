#include "obj_loader.h"

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
            std::string face{temp_stream.str()};

            face.replace(face.begin(), face.begin() + 2, "");

            faces.push_back(face);
        }
    }


    return {positions, text_coordinates, normals, faces};
}

std::vector<Vertex> Obj_Loader::load_obj(const std::string &file_path) {
    Obj_Data object_data = load_file_(file_path);
    std::vector<Vertex> vertices;

    for (auto &face: object_data.faces) {
        std::vector<std::string> face_vertices;
        boost::split(face_vertices, face, boost::is_any_of(" "));

        for (auto &vertex: face_vertices) {
            std::vector<std::string> indices;
            glm::vec2 texture_coordinate{0.f};

            boost::split(indices, vertex, boost::is_any_of("/"));

            if (!indices[1].empty())
                texture_coordinate = object_data.text_coordinates[std::stoi(indices[1]) - 1];

            vertices.push_back(
                    {
                            object_data.positions[std::stoi(indices[0]) - 1],
                            glm::vec3{1.f, 1.f, 1.f},
                            texture_coordinate,
                            object_data.normals[std::stoi(indices[2]) - 1],
                    }
            );
        }
    }

    return vertices;
}
