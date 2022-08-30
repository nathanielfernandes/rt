#include "data.hpp"
#include "shader.hpp"
#include <Shadinclude.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glu.hpp>
#include <iostream>
#include <program.hpp>
#include <renderer.hpp>
#include <scene.hpp>
#include <string>

Renderer::Renderer(Scene *scene)
    : initialized(false), scene(scene), resolution(scene->options.resolution),
      depthMax(scene->options.depth), trianglesBuffer(0), verticesBuffer(0),
      normalsBuffer(0), trianglesTex(0), verticesTex(0), normalsTex(0),
      ptShader(nullptr), tmShader(nullptr), outputShader(nullptr), ptFBO(0),
      accFBO(0), outputFBO(0), ptTex(0), accTex(0), outputTex(0) {

  if (!scene->initialized)
    scene->ProcessScene();

  initDataBuffers();

  surface = new Surface();

  initFBOs();
  loadShaders();
  initShaders();
}

Renderer::~Renderer() {
  delete surface;

  // Delete data buffers and textures
  glDeleteBuffers(1, &BVHBuffer);
  glDeleteBuffers(1, &trianglesBuffer);
  glDeleteBuffers(1, &verticesBuffer);
  glDeleteBuffers(1, &normalsBuffer);

  glDeleteTextures(1, &BVHTex);
  glDeleteTextures(1, &trianglesTex);
  glDeleteTextures(1, &verticesTex);
  glDeleteTextures(1, &normalsTex);

  // Delete render textures
  glDeleteTextures(1, &ptTex);
  glDeleteTextures(1, &accTex);
  glDeleteTextures(1, &outputTex);

  // Delete frame buffers
  glDeleteFramebuffers(1, &ptFBO);
  glDeleteFramebuffers(1, &accFBO);
  glDeleteFramebuffers(1, &outputFBO);

  // Delete shaders
  delete ptShader;
  delete tmShader;
  delete outputShader;
}

void Renderer::initDataBuffers() {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  glGenBuffers(1, &BVHBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, BVHBuffer);
  glBufferData(GL_TEXTURE_BUFFER,
               scene->bvhTranslator.nodes.size() *
                   sizeof(RadeonRays::BvhTranslator::Node),
               &scene->bvhTranslator.nodes[0], GL_STATIC_DRAW);
  glGenTextures(1, &BVHTex);
  glBindTexture(GL_TEXTURE_BUFFER, BVHTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, BVHBuffer);

  glGenBuffers(1, &trianglesBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, trianglesBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->triangles.size() * sizeof(Triangle),
               &scene->triangles[0], GL_STATIC_DRAW);
  glGenTextures(1, &trianglesTex);
  glBindTexture(GL_TEXTURE_BUFFER, trianglesTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, trianglesBuffer);

  glGenBuffers(1, &verticesBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, verticesBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->vertices.size() * sizeof(VertexU),
               &scene->vertices[0], GL_STATIC_DRAW);
  glGenTextures(1, &verticesTex);
  glBindTexture(GL_TEXTURE_BUFFER, verticesTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, verticesBuffer);

  glGenBuffers(1, &normalsBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, normalsBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->normals.size() * sizeof(NormalV),
               &scene->normals[0], GL_STATIC_DRAW);
  glGenTextures(1, &normalsTex);
  glBindTexture(GL_TEXTURE_BUFFER, normalsTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, normalsBuffer);

  // glGenTextures(1, &materialsTex);
  // glBindTexture(GL_TEXTURE_2D, materialsTex);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, scene->materials.size() * 2, 1,
  // 0,
  //              GL_RGBA, GL_FLOAT, &scene->materials[0]);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glBindTexture(GL_TEXTURE_2D, 0);

  glGenBuffers(1, &materialsBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, materialsBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->materials.size() * sizeof(Material),
               &scene->materials[0], GL_STATIC_DRAW);
  glGenTextures(1, &materialsTex);
  glBindTexture(GL_TEXTURE_BUFFER, materialsTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, materialsBuffer);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_BUFFER, BVHTex);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_BUFFER, trianglesTex);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_BUFFER, verticesTex);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_BUFFER, normalsTex);
  glActiveTexture(GL_TEXTURE5);
  glBindTexture(GL_TEXTURE_BUFFER, materialsTex);

  std::cout << "Triangles: " << scene->bvhTranslator.nodes.size() << std::endl;
}

void Renderer::loadShaders() {
  vertexShaderSrc = Shadinclude::load("shaders/vertex.glsl");
  ptShaderSrc = Shadinclude::load("shaders/pt.frag");
  tmShaderSrc = Shadinclude::load("shaders/tm.frag");
  outputShaderSrc = Shadinclude::load("shaders/output.frag");

  std::cout << "Loaded shaders" << std::endl;
}

void Renderer::reloadShaders() {
  // Delete shaders
  delete ptShader;
  delete tmShader;
  delete outputShader;

  initShaders();
}

void Renderer::initShaders() {
  ptShader = buildProgram(vertexShaderSrc, ptShaderSrc);
  tmShader = buildProgram(vertexShaderSrc, tmShaderSrc);
  outputShader = buildProgram(vertexShaderSrc, outputShaderSrc);

  GLuint shaderObj;
  ptShader->use();
  shaderObj = ptShader->object();

  glUniform2f(glGetUniformLocation(shaderObj, "resolution"),
              float(resolution.x), float(resolution.y));

  glUniform1i(glGetUniformLocation(shaderObj, "accTex"), 0);
  glUniform1i(glGetUniformLocation(shaderObj, "BVHTex"), 1);
  glUniform1i(glGetUniformLocation(shaderObj, "trianglesTex"), 2);
  glUniform1i(glGetUniformLocation(shaderObj, "verticesTex"), 3);
  glUniform1i(glGetUniformLocation(shaderObj, "normalsTex"), 4);
  glUniform1i(glGetUniformLocation(shaderObj, "materialsTex"), 5);

  ptShader->unuse();

  std::cout << "Initialized shaders" << std::endl;
}

void Renderer::reloadAcc() {
  sampleCount = 1;
  frameCount = 1;
  resolution = scene->options.resolution;

  glDeleteTextures(1, &accTex);
  glDeleteFramebuffers(1, &accFBO);

  glGenFramebuffers(1, &accFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, accFBO);

  glGenTextures(1, &accTex);
  glBindTexture(GL_TEXTURE_2D, accTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, resolution.x, resolution.y, 0,
               GL_RGBA, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         accTex, 0);
}

void Renderer::initFBOs() {
  sampleCount = 1;
  frameCount = 1;

  resolution = scene->options.resolution;

  glGenFramebuffers(1, &ptFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, ptFBO);

  glGenTextures(1, &ptTex);
  glBindTexture(GL_TEXTURE_2D, ptTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, resolution.x, resolution.y, 0,
               GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         ptTex, 0);

  // Accumulation buffer
  glGenFramebuffers(1, &accFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, accFBO);

  glGenTextures(1, &accTex);
  glBindTexture(GL_TEXTURE_2D, accTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, resolution.x, resolution.y, 0,
               GL_RGBA, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         accTex, 0);

  // Output buffer
  glGenFramebuffers(1, &outputFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);

  glGenTextures(1, &outputTex);
  glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
}

void Renderer::render() {
  glActiveTexture(GL_TEXTURE0);
  glBindFramebuffer(GL_FRAMEBUFFER, ptFBO);
  glViewport(0, 0, resolution.x, resolution.y);
  glBindTexture(GL_TEXTURE_2D, accTex);
  surface->Draw(ptShader);

  glBindFramebuffer(GL_FRAMEBUFFER, accFBO);
  glViewport(0, 0, resolution.x, resolution.y);
  glBindTexture(GL_TEXTURE_2D, ptTex);
  surface->Draw(outputShader);

  glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         ptTex, 0);
  glViewport(0, 0, resolution.x, resolution.y);
  glBindTexture(GL_TEXTURE_2D, accTex);
  surface->Draw(tmShader);
}

void Renderer::draw() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ptTex);
  surface->Draw(tmShader);
}

void Renderer::update(float t) {
  // if (!initialized) {
  //   return;
  // }

  GLuint shaderObject;

  ptShader->use();
  shaderObject = ptShader->object();
  glUniform1i(glGetUniformLocation(shaderObject, "depthMax"), depthMax);
  glUniform1i(glGetUniformLocation(shaderObject, "bvhTop"),
              scene->bvhTranslator.topLevelIndex);
  glUniform1i(glGetUniformLocation(shaderObject, "frameNum"), sampleCount);
  glUniform1f(glGetUniformLocation(shaderObject, "time"), t);
  // update camera
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.origin"), 1,
               glm::value_ptr(scene->camera->origin));
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.llc"), 1,
               glm::value_ptr(scene->camera->llc));
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.horizontal"), 1,
               glm::value_ptr(scene->camera->horizontal));
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.vertical"), 1,
               glm::value_ptr(scene->camera->vertical));
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.u"), 1,
               glm::value_ptr(scene->camera->u));
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.v"), 1,
               glm::value_ptr(scene->camera->v));
  glUniform3fv(glGetUniformLocation(shaderObject, "camera.w"), 1,
               glm::value_ptr(scene->camera->w));

  glUniform1f(glGetUniformLocation(shaderObject, "camera.lens_radius"),
              scene->camera->lens_radius);

  ptShader->unuse();

  tmShader->use();
  shaderObject = tmShader->object();
  glUniform1f(glGetUniformLocation(shaderObject, "invSampleNum"),
              (1.0f / (float)sampleCount));
  // glUniform1i(glGetUniformLocation(shaderObject, "ptTex"), 1);
  tmShader->unuse();

  sampleCount++;
}
