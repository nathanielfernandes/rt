
#include "glm/ext/vector_float3.hpp"
#include <surface.hpp>

Surface::Surface() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Vertex data
  glm::vec3 vertices[] = {
      glm::vec3{-1.0f, 1.0f, 0.0f}, glm::vec3{1.0f, -1.0f, -1.0f},
      glm::vec3{0.0f, 0.0f, 1.0f},  glm::vec3{-1.0f, 1.0f, 0.0f},
      glm::vec3{-1.0f, 1.0f, 0.0f}, glm::vec3{1.0f, 1.0f, -1.0f},
      glm::vec3{1.0f, 0.0f, 1.0f},  glm::vec3{1.0f, 1.0f, 1.0f}};

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)(2 * sizeof(GLfloat)));

  glBindVertexArray(0);
}

void Surface::Draw(Program *program) {
  program->use();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
  program->unuse();
}