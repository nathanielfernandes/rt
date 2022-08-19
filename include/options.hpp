#pragma once

#include "glm/glm.hpp"

struct Options {
  Options() {
    samples = 64;
    depth = 50;
    resolution = glm::ivec2(800, 600);
  }

  int samples;
  int depth;
  glm::ivec2 resolution;
};