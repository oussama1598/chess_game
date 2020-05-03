#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>

#include "mesh.h"

class Obj_Loader {
private:
    struct Obj_Data {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> text_coordinates;
        std::vector<glm::vec3> normals;
        std::vector<std::string> faces;
    };

    static Obj_Data load_file_(const std::string &file_path);

public:
    static std::vector<Mesh::Vertex> load_obj(const std::string &file_path);
};
