#include <scene.hpp>

void Scene::setCamera(Camera *camera) { this->camera = camera; }

// https://github.com/knightcrawler25/GLSL-PathTracer/blob/master/src/core/Scene.cpp#L62
int Scene::AddMesh(const std::string &fp) {
  int id = -1;

  for (int i = 0; i < meshes.size(); i++)
    if (meshes[i]->name == fp)
      return i;

  id = meshes.size();
  Mesh *mesh = new Mesh;

  printf("Loading model %s\n", fp.c_str());
  if (mesh->LoadFromFile(fp))
    meshes.push_back(mesh);
  else {
    printf("Unable to load model %s\n", fp.c_str());
    delete mesh;
    id = -1;
  }
  return id;
}

int Scene::AddMeshInstance(const MeshInstance &meshInstance) {
  int id = meshInstances.size();
  meshInstances.push_back(meshInstance);
  return id;
}

void Scene::createTLAS() {
  // Loop through all the mesh Instances and build a Top Level BVH
  std::vector<RadeonRays::bbox> bounds;
  bounds.resize(meshInstances.size());

  for (int i = 0; i < meshInstances.size(); i++) {

    RadeonRays::bbox bbox = meshes[meshInstances[i].meshID]->bvh->Bounds();
    glm::mat4 matrix = meshInstances[i].transform;

    glm::vec3 minBound = bbox.pmin;
    glm::vec3 maxBound = bbox.pmax;

    glm::vec3 right = glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]);
    glm::vec3 up = glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
    glm::vec3 forward = glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
    glm::vec3 translation = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);

    glm::vec3 xa = right * minBound.x;
    glm::vec3 xb = right * maxBound.x;

    glm::vec3 ya = up * minBound.y;
    glm::vec3 yb = up * maxBound.y;

    glm::vec3 za = forward * minBound.z;
    glm::vec3 zb = forward * maxBound.z;

    minBound =
        glm::min(xa, xb) + glm::min(ya, yb) + glm::min(za, zb) + translation;
    maxBound =
        glm::max(xa, xb) + glm::max(ya, yb) + glm::max(za, zb) + translation;

    RadeonRays::bbox bound;
    bound.pmin = minBound;
    bound.pmax = maxBound;

    bounds[i] = bound;
  }

  sceneBvh->Build(&bounds[0], bounds.size());

  sceneBounds = sceneBvh->Bounds();
}

void Scene::createBLAS() {
  // Loop through all meshes and build BVHs
#pragma omp parallel for
  for (int i = 0; i < meshes.size(); i++) {
    printf("Building BVH for %s\n", meshes[i]->name.c_str());
    meshes[i]->BuildBVH();
  }
}

void Scene::ProcessScene() {
  printf("Processing scene data\n");
  createBLAS();

  printf("Building scene BVH\n");
  createTLAS();

  // Flatten BVH
  printf("Flattening BVH\n");
  bvhTranslator.Process(sceneBvh, meshes, meshInstances);

  // Copy mesh data
  int verticesCnt = 0;
  printf("Copying Mesh Data\n");
  for (int i = 0; i < meshes.size(); i++) {
    // Copy indices from BVH and not from Mesh.
    // Required if splitBVH is used as a triangle can be shared by leaf nodes
    int numIndices = meshes[i]->bvh->GetNumIndices();
    const int *triIndices = meshes[i]->bvh->GetIndices();

    for (int j = 0; j < numIndices; j++) {
      int index = triIndices[j];
      int v1 = (index * 3 + 0) + verticesCnt;
      int v2 = (index * 3 + 1) + verticesCnt;
      int v3 = (index * 3 + 2) + verticesCnt;

      triangles.push_back(Triangle(v1, v2, v3));
    }

    vertices.insert(vertices.end(), meshes[i]->verticesU.begin(),
                    meshes[i]->verticesU.end());
    normals.insert(normals.end(), meshes[i]->normalsV.begin(),
                   meshes[i]->normalsV.end());

    verticesCnt += meshes[i]->verticesU.size();
  }

  // Copy transforms
  //   printf("Copying transforms\n");
  //   transforms.resize(meshInstances.size());
  //   for (int i = 0; i < meshInstances.size(); i++)
  //     transforms[i] = meshInstances[i].transform;

  //   // Copy textures
  //   if (!textures.empty())
  //     printf("Copying and resizing textures\n");

  //   int reqWidth = renderOptions.texArrayWidth;
  //   int reqHeight = renderOptions.texArrayHeight;
  //   int texBytes = reqWidth * reqHeight * 4;
  //   textureMapsArray.resize(texBytes * textures.size());

  // #pragma omp parallel for
  //   for (int i = 0; i < textures.size(); i++) {
  //     int texWidth = textures[i]->width;
  //     int texHeight = textures[i]->height;

  //     // Resize textures to fit 2D texture array
  //     if (texWidth != reqWidth || texHeight != reqHeight) {
  //       unsigned char *resizedTex = new unsigned char[texBytes];
  //       stbir_resize_uint8(&textures[i]->texData[0], texWidth, texHeight, 0,
  //                          resizedTex, reqWidth, reqHeight, 0, 4);
  //       std::copy(resizedTex, resizedTex + texBytes,
  //                 &textureMapsArray[i * texBytes]);
  //       delete[] resizedTex;
  //     } else
  //       std::copy(textures[i]->texData.begin(), textures[i]->texData.end(),
  //                 &textureMapsArray[i * texBytes]);
  //   }

  // Add a default camera
  //   if (!camera) {
  //     RadeonRays::bbox bounds = sceneBvh->Bounds();
  //     glm::vec3 extents = bounds.extents();
  //     glm::vec3 center = bounds.center();
  //     AddCamera(
  //         glm::vec3(center.x, center.y, center.z + glm::length(extents)
  //         * 2.0f), center, 45.0f);
  //   }

  initialized = true;
}