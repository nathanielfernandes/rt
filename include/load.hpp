
#pragma once

#include "data.hpp"
#include <string>
#include <vector>

bool LoadModel(const std::string &fp, std::vector<Triangle> &triangles,
               std::vector<VertexU> &vertices, std::vector<NormalV> &normals,
               std::vector<Material> &materials, std::vector<std::string> &mat_names);
