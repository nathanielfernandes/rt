/*
 * MIT License
 *
 * Copyright(c) 2019 Asif Ali
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "tiny_obj_loader.h"
#include <iostream>
#include <load.hpp>
#include <mesh.hpp>

float sphericalTheta(const glm::vec3 &v) {
  return acosf(std::clamp(v.y, -1.f, 1.f));
}

float sphericalPhi(const glm::vec3 &v) {
  float p = atan2f(v.z, v.x);
  return (p < 0.f) ? p + 2.f * M_PI : p;
}

bool Mesh::LoadFromFile(const std::string &fp) {
  name = fp;
  return LoadModel(fp, triangles, verticesU, normalsV, materials);
}

void Mesh::BuildBVH() {
  const int numTris = verticesU.size() / 3;
  std::vector<RadeonRays::bbox> bounds(numTris);

#pragma omp parallel for
  for (int i = 0; i < numTris; ++i) {
    const glm::vec3 v1 = glm::vec3(verticesU[i * 3 + 0]);
    const glm::vec3 v2 = glm::vec3(verticesU[i * 3 + 1]);
    const glm::vec3 v3 = glm::vec3(verticesU[i * 3 + 2]);

    bounds[i].grow(v1);
    bounds[i].grow(v2);
    bounds[i].grow(v3);
  }

  bvh->Build(&bounds[0], numTris);
}
