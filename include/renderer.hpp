#pragma once

#include "scene.hpp"
#include <program.hpp>
#include <surface.hpp>

class Renderer {
private:
  const Scene *scene;

  GLuint trianglesTex, verticesTex, normalsTex;
  GLuint triangleBuffer, vertexBuffer, normalBuffer;

  Surface *surface;
  int totalLights;

  GLuint ptFBO, accFBO;
  GLuint ptTex, accTex;

  Program *ptProgram, *accProgram, *postprocessProgram;
  int sampleCount;
  bool initialized;

public:
  Renderer(const Scene *scene);
  ~Renderer();
  glm::ivec2 screen;

  void init();
  void genBuffers();
  void loadShaders();
  void setUniforms();

  void finish();
  void render();
  void draw();
  void update(float dt);
  float progress();
};