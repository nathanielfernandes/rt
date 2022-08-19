
#pragma once

#include "data.hpp"
#include <string.h>
#include <vector>

bool LoadModel(const std::string &fp, std::vector<VertexU> &vertices,
               std::vector<NormalV> &normals);
