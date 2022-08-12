#pragma once

#include <glu.hpp>
#include <program.hpp>

class Surface {
public:
  Surface();
  void Draw(Program *);

private:
  GLuint vao, vbo;
};
