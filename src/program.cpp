#include <program.hpp>

Program::Program(const std::vector<Shader> &shaders) {
  _object = glCreateProgram();
  for (const auto &shader : shaders) {
    glAttachShader(_object, shader.object());
  }

  glLinkProgram(_object);

  GLint isLinked = 0;
  glGetProgramiv(_object, GL_LINK_STATUS, (int *)&isLinked);

  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(_object, maxLength, &maxLength, &infoLog[0]);
    glDeleteProgram(_object);
    throw std::runtime_error(std::string(infoLog.begin(), infoLog.end()));
  }

  for (const auto &shader : shaders) {
    glDetachShader(_object, shader.object());
  }
}

Program *Program::build(const std::string &vs_path,
                        const std::string &fs_path) {
  std::vector<Shader> shaders;
  shaders.push_back(Shader(vs_path, GL_VERTEX_SHADER));
  shaders.push_back(Shader(fs_path, GL_FRAGMENT_SHADER));
  return new Program(shaders);
}

Program::~Program() { glDeleteProgram(_object); }

void Program::use() { glUseProgram(_object); }

void Program::unuse() { glUseProgram(0); }
