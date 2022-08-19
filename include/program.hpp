#pragma once

#include <shader.hpp>
#include <vector>

class Program {
public:
  Program(const std::vector<Shader> &shaders);
  ~Program();

  void use();
  void unuse();

  GLuint object() const { return _object; }

private:
  GLuint _object;
  std::vector<Shader> _shaders;
};

Program *buildProgram(const std::string &vs_src, const std::string &fs_src);
