#include <shader.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

int shaderCount = -1;

Shader::Shader(const std::string &src, GLuint shaderType) {
  // _object = glCreateShader(shaderType);
  // std::ifstream file(filePath);
  // if (!file.is_open()) {
  //   std::cout << "Failed to open shader: " << filePath << std::endl;
  //   return;
  // }
  // std::stringstream stream;
  // stream << file.rdbuf();
  // file.close();
  // std::string contents = stream.str();
  // const char *source = contents.c_str();
  // glShaderSource(_object, 1, &source, nullptr);
  // glCompileShader(_object);
  // GLint isCompiled = 0;
  // glGetShaderiv(_object, GL_COMPILE_STATUS, &isCompiled);
  // if (isCompiled == GL_FALSE) {
  //   GLint maxLength = 0;
  //   glGetShaderiv(_object, GL_INFO_LOG_LENGTH, &maxLength);
  //   char *info = new char[maxLength + 1];
  //   glGetShaderInfoLog(_object, maxLength, NULL, info);
  //   glDeleteShader(_object);
  //   std::cout << "Failed to compile shader: " << filePath << std::endl;
  //   std::cout << info << std::endl;
  //   return;
  // }
  shaderCount++;

  _object = glCreateShader(shaderType);
  const char *source = src.c_str();
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
    std::cout << "Failed to compile shader: " << shaderCount << std::endl;
    std::cout << info << std::endl;
    return;
  }
}
