#include <shader.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string &filePath, GLuint shaderType) {
  _object = glCreateShader(shaderType);
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    return;
  }
  std::stringstream stream;
  stream << file.rdbuf();
  file.close();
  std::string contents = stream.str();
  const char *source = contents.c_str();
  glShaderSource(_object, 1, &source, nullptr);
  glCompileShader(_object);
  GLint isCompiled = 0;
  glGetShaderiv(_object, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &maxLength);
    char *info = new char[maxLength + 1];
    glGetShaderInfoLog(_object, maxLength, NULL, info);
    glDeleteShader(_object);
    std::cerr << "Failed to compile shader: " << filePath << std::endl;
    std::cerr << info << std::endl;
    return;
  }
}
