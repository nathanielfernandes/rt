#include "data.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>

#include <load.hpp>
#include <tiny_obj_loader.h>

bool LoadModel(const std::string &fp, std::vector<VertexU> &vertices,
               std::vector<NormalV> &normals) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn;
  std::string err;

  float materialId = 0;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              fp.c_str(), 0, true);

  if (!ret) {
    std::cout << err << std::endl;
    return false;
  }

  for (const auto &shape : shapes) {
    int offset = 0;
    for (const auto &index : shape.mesh.indices) {

      // check for texture coords, if not present, calculate them
      glm::vec2 uv;
      if (!attrib.texcoords.empty()) {
        uv = glm::vec2(attrib.texcoords[index.texcoord_index * 2],
                       1.0f - attrib.texcoords[index.texcoord_index * 2 + 1]);
      } else {
        uv = glm::vec2(0.0f, 0.0f);
      }

      vertices.push_back(VertexU(attrib.vertices[3 * index.vertex_index + 0],
                                 attrib.vertices[3 * index.vertex_index + 1],
                                 attrib.vertices[3 * index.vertex_index + 2],
                                 uv.x));

      // if there is no normal, calculate it
      if (attrib.normals.empty()) {
        glm::vec3 normal = glm::normalize(
            glm::cross(glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
                                 attrib.vertices[3 * index.vertex_index + 1],
                                 attrib.vertices[3 * index.vertex_index + 2]),
                       glm::vec3(attrib.vertices[3 * index.vertex_index + 3],
                                 attrib.vertices[3 * index.vertex_index + 4],
                                 attrib.vertices[3 * index.vertex_index + 5])));
        normals.push_back(NormalV(normal.x, normal.y, normal.z, uv.y));
      } else {
        normals.push_back(NormalV(attrib.normals[3 * index.normal_index + 0],
                                  attrib.normals[3 * index.normal_index + 1],
                                  attrib.normals[3 * index.normal_index + 2],
                                  uv.y));
      }
    }
  }

  // for (int i = 0; i < scene->vertices.size(); i += 3) {
  //   scene->triangles.push_back(Triangle(i, i + 1, i + 2));
  // }

  return true;
}

// bool LoadModel(Scene *scene, const std::string &fp) {
//   tinyobj::attrib_t attrib;
//   std::vector<tinyobj::shape_t> shapes;
//   std::vector<tinyobj::material_t> materials;
//   std::string warn;
//   std::string err;

//   float materialId = 0;

//   bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
//                               fp.c_str(), 0, true);

//   if (!ret) {
//     std::cout << err << std::endl;
//     return false;
//   }

//   int vertCount = int(attrib.vertices.size() / 3);
//   size_t vertStartIndex = scene->vertices.size();
//   for (int i = 0; i < vertCount; i++)
//     scene->vertices.push_back(
//         Vertex{glm::vec3(attrib.vertices[3 * i + 0], attrib.vertices[3 * i +
//         1],
//                          attrib.vertices[3 * i + 2])});

//   // Loop over shapes
//   for (size_t s = 0; s < shapes.size(); s++) {
//     // Loop over faces(polygon)
//     int index_offset = 0;

//     for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
//       glm::vec4 indices;
//       glm::vec3 v[3], n[3], t[3];
//       bool hasNormals = true;
//       float vx, vy, vz, nx, ny, nz, tx, ty;

//       int fv = shapes[s].mesh.num_face_vertices[f];
//       for (int i = 0; i < fv; i++) {
//         tinyobj::index_t idx =
//             tinyobj::index_t(shapes[s].mesh.indices[index_offset + i]);
//         indices[i] = float(vertStartIndex + (3.0 * idx.vertex_index + i)
//         / 3.0);

//         vx = attrib.vertices[3 * idx.vertex_index + 0];
//         vy = attrib.vertices[3 * idx.vertex_index + 1];
//         vz = attrib.vertices[3 * idx.vertex_index + 2];

//         // Normals
//         if (idx.normal_index != -1) {
//           nx = attrib.normals[3 * idx.normal_index + 0];
//           ny = attrib.normals[3 * idx.normal_index + 1];
//           nz = attrib.normals[3 * idx.normal_index + 2];
//         } else {
//           hasNormals = false;
//         }

//         // TexCoords
//         if (idx.texcoord_index != -1) {
//           tx = attrib.texcoords[2 * idx.texcoord_index + 0];
//           ty = 1.0f - attrib.texcoords[2 * idx.texcoord_index + 1];
//         } else {
//           // If model has no texture Coords then default to 0
//           tx = ty = 0;
//         }

//         n[i] = glm::vec3(nx, ny, nz);
//         t[i] = glm::vec3(tx, ty, materialId);
//       }
//       if (!hasNormals) {
//         // If model has no normals then Generate flat normals
//         glm::vec3 flatNormal =
//             glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
//         n[0] = n[1] = n[2] = flatNormal;
//       }

//       scene->triangles.push_back(Triangle{indices});
//       scene->normals.push_back(Normal{n[0], n[1], n[2]});

//       index_offset += fv;
//     }
//   }

//   return true;
// }

// bool LoadModel(Scene *scene, const std::string &fp) {
//   tinyobj::attrib_t attrib;
//   std::vector<tinyobj::shape_t> shapes;
//   std::vector<tinyobj::material_t> materials;
//   std::string warn;
//   std::string err;

//   float materialId = 0;

//   bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
//                               fp.c_str(), 0, true);

//   if (!ret) {
//     std::cout << err << std::endl;
//     return false;
//   }

//   // Loop over shapes
//   for (size_t s = 0; s < shapes.size(); s++) {
//     // Loop over faces(polygon)
//     size_t index_offset = 0;

//     for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
//       // Loop over vertices in the face.
//       for (size_t v = 0; v < 3; v++) {
//         // access to vertex
//         tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
//         tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
//         tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
//         tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
//         tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
//         tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
//         tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

//         tinyobj::real_t tx, ty;

//         // if (!attrib.texcoords.empty()) {
//         //   tx = attrib.texcoords[2 * idx.texcoord_index + 0];
//         //   ty = 1.0 - attrib.texcoords[2 * idx.texcoord_index + 1];
//         // } else {
//         //   if (v == 0)
//         //     tx = ty = 0;
//         //   else if (v == 1)
//         //     tx = 0, ty = 1;
//         //   else
//         //     tx = ty = 1;
//         // }

//         scene->vertices.push_back(Vertex{glm::vec3{vx, vy, vz}});
//         scene->normals.push_back(Normal{glm::vec3{nx, ny, nz}});
//       }

//       index_offset += 3;
//     }
//   }

//   return true;
// }