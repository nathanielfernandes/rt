#pragma once

#include <program.hpp>
#include <scene.hpp>
#include <surface.hpp>

class Renderer {
private:
  Surface *surface;

  // Data buffers and textures
  GLuint trianglesTex, verticesTex, normalsTex, materialsTex, BVHTex;
  GLuint trianglesBuffer, verticesBuffer, normalsBuffer, materialsBuffer,
      BVHBuffer;

  // Shader sources
  std::string vertexShaderSrc, ptShaderSrc, tmShaderSrc, outputShaderSrc;

  // Shaders
  Program *ptShader, *tmShader, *outputShader;

  // Frame buffers
  GLuint ptFBO, accFBO, outputFBO;

  // Render Texures
  GLuint ptTex, accTex, outputTex;

  int frameCount;
  bool initialized;

  void loadShaders();
  void initFBOs();
  void initShaders();
  void initDataBuffers();
  void setUniforms(GLuint shaderObject);

public:
  int sampleCount;
  int depthMax;

  Renderer(Scene *scene);
  Scene *scene;
  void reloadShaders();
  void reloadAcc();
  void reloadMaterials();

  ~Renderer();

  glm::ivec2 resolution;

  void render();
  void draw();
  void update(float dt);
  float progress();
};