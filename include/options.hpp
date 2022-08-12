#pragma once

#include "glm/glm.hpp"

struct Options {
  Options() {
    samples = 64;
    depth = 4;
    resolution = glm::ivec2(640, 480);
  }

  int samples;
  int depth;
  glm::ivec2 resolution;
};