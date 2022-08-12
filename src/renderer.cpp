#include <Shadinclude.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glu.hpp>
#include <iostream>
#include <program.hpp>
#include <renderer.hpp>
#include <scene.hpp>
#include <string>

Renderer::Renderer(const Scene *scene)
    : initialized(false), scene(scene), screen(scene->options.resolution) {}

Renderer::~Renderer() {
  if (initialized)
    this->finish();
}

void Renderer::finish() {
  if (initialized) {
    glDeleteFramebuffers(1, &ptFBO);
    glDeleteFramebuffers(1, &accFBO);

    glDeleteTextures(1, &ptTex);
    glDeleteTextures(1, &accTex);

    delete ptProgram;
    delete accProgram;
    delete postprocessProgram;

    glDeleteTextures(1, &trianglesTex);
    glDeleteTextures(1, &verticesTex);
    glDeleteTextures(1, &normalsTex);

    glDeleteBuffers(1, &triangleBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &normalBuffer);

    delete surface;
    initialized = false;
    std::cout << "Renderer finished" << std::endl;
  }
}

void Renderer::genBuffers() {
  // Gen Triangles Buffer
  glGenBuffers(1, &triangleBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, triangleBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->triangles.size() * sizeof(Triangle),
               &scene->triangles[0], GL_STATIC_DRAW);

  // Gen Triangles Texure
  glGenTextures(1, &trianglesTex);
  glBindTexture(GL_TEXTURE_BUFFER, trianglesTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, triangleBuffer);

  // Gen Vertices Buffer
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, vertexBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->vertices.size() * sizeof(Vertex),
               &scene->vertices[0], GL_STATIC_DRAW);

  // Gen Vertices Texure
  glGenTextures(1, &verticesTex);
  glBindTexture(GL_TEXTURE_BUFFER, verticesTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vertexBuffer);

  // Gen Materials Buffer
  // glGenBuffers(1, &materialBuffer);
  // glBindBuffer(GL_TEXTURE_BUFFER, materialBuffer);
  // glBufferData(GL_TEXTURE_BUFFER, scene->materials.size() * sizeof(Material),
  //              &scene->materials[0], GL_STATIC_DRAW);

  // // Gen Materials Texure
  // glGenTextures(1, &materialsTex);
  // glBindTexture(GL_TEXTURE_BUFFER, materialsTex);
  // glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, materialBuffer);

  // // Gen Lights Buffer
  // totalLights = int(scene->lights.size());
  // if (totalLights > 0) {
  //   glGenBuffers(1, &lightBuffer);
  //   glBindBuffer(GL_TEXTURE_BUFFER, lightBuffer);
  //   glBufferData(GL_TEXTURE_BUFFER, scene->lights.size() * sizeof(Light),
  //                &scene->lights[0], GL_STATIC_DRAW);

  //   // Gen Lights Texure
  //   glGenTextures(1, &lightsTex);
  //   glBindTexture(GL_TEXTURE_BUFFER, lightsTex);
  //   glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, lightBuffer);
  // }

  // Gen Normals Buffer
  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_TEXTURE_BUFFER, normalBuffer);
  glBufferData(GL_TEXTURE_BUFFER, scene->normals.size() * sizeof(Normal),
               &scene->normals[0], GL_STATIC_DRAW);

  // Gen Normals Texure
  glGenTextures(1, &normalsTex);
  glBindTexture(GL_TEXTURE_BUFFER, normalsTex);
  glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, normalBuffer);

  // Framebuffers
  glGenFramebuffers(1, &ptFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, ptFBO);

  glGenTextures(1, &ptTex);
  glBindTexture(GL_TEXTURE_2D, ptTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screen.x, screen.y, 0, GL_RGB,
               GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         ptTex, 0);

  glGenFramebuffers(1, &accFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, accFBO);

  glGenTextures(1, &accTex);
  glBindTexture(GL_TEXTURE_2D, accTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screen.x, screen.y, 0, GL_RGB,
               GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         accTex, 0);
}

void Renderer::loadShaders() {
  const std::string ptShaderVert = Shadinclude::load("shaders/main.vert");
  const std::string ptShaderFrag = Shadinclude::load("shaders/main.frag");

  const std::string accShaderVert = Shadinclude::load("shaders/acc.vert");
  const std::string accShaderFrag = Shadinclude::load("shaders/acc.frag");

  const std::string postprocessShaderVert =
      Shadinclude::load("shaders/postprocess.vert");
  const std::string postprocessShaderFrag =
      Shadinclude::load("shaders/postprocess.frag");

  ptProgram = buildProgram(ptShaderVert, ptShaderFrag);
  accProgram = buildProgram(accShaderVert, accShaderFrag);
  postprocessProgram =
      buildProgram(postprocessShaderVert, postprocessShaderFrag);
}

void Renderer::setUniforms() {
  GLuint shaderObject;
  ptProgram->use();
  shaderObject = ptProgram->object();

  glUniform1i(glGetUniformLocation(shaderObject, "maxDepth"),
              scene->options.depth);
  glUniform2f(glGetUniformLocation(shaderObject, "screenResolution"),
              float(screen.x), float(screen.y));
  glUniform1i(glGetUniformLocation(shaderObject, "totalLights"), totalLights);

  glUniform1i(glGetUniformLocation(shaderObject, "accTex"), 0);
  glUniform1i(glGetUniformLocation(shaderObject, "trianglesTex"), 1);
  glUniform1i(glGetUniformLocation(shaderObject, "verticesTex"), 2);
  glUniform1i(glGetUniformLocation(shaderObject, "normalsTex"), 3);
  // glUniform1i(glGetUniformLocation(shaderObject, "materialsTex"), 4);
  // glUniform1i(glGetUniformLocation(shaderObject, "lightsTex"), 5);

  ptProgram->unuse();
}

void Renderer::init() {
  if (initialized) {
    std::cout << "Renderer already initialized" << std::endl;
    return;
  }

  sampleCount = 1;
  surface = new Surface();

  genBuffers();
  loadShaders();
  setUniforms();

  initialized = true;
}

void Renderer::render() {
  if (!initialized) {
    return;
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, accTex);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_BUFFER, trianglesTex);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_BUFFER, verticesTex);

  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_BUFFER, normalsTex);

  glBindFramebuffer(GL_FRAMEBUFFER, ptFBO);
  surface->Draw(ptProgram);

  glBindFramebuffer(GL_FRAMEBUFFER, accFBO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ptTex);
  surface->Draw(accProgram);
}

void Renderer::draw() {
  if (!initialized) {
    return;
  }

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, accTex);
  surface->Draw(postprocessProgram);
}

void Renderer::update(float dt) {
  if (!initialized) {
    return;
  }

  GLuint shaderObject;

  ptProgram->use();
  shaderObject = ptProgram->object();
  // update camera

  ptProgram->unuse();

  postprocessProgram->use();
  shaderObject = postprocessProgram->object();
  glUniform1f(glGetUniformLocation(shaderObject, "invSampleCounter"),
              (1.0f / sampleCount));
  postprocessProgram->unuse();
}
