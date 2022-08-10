#pragma once

#include <shader.hpp>
#include <vector>

class Program {
public:
  Program(const std::vector<Shader> &shaders);
  ~Program();

  Program *build(const std::string &vs_path, const std::string &fs_path);

  void use();
  void unuse();

  GLuint object() const { return _object; }

private:
  GLuint _object;
  std::vector<Shader> _shaders;
};