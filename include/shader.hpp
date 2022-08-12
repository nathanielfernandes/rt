#pragma once

#include <glu.hpp>
#include <string>

class Shader {
private:
  GLuint _object;

public:
  Shader(const std::string &filePath, GLuint shaderType);
  GLuint object() const { return _object; }
};
