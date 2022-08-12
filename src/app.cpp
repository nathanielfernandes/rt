#include <app.hpp>
#include <iostream>

App::App(Scene *scene) : scene(scene) {}
App::~App() {
  if (initialized) {
    finish();
  }
}

void App::init() {
  if (!glfwInit()) {
    fprintf(stdout, "ERROR: could not start GLFW3\n");
    return;
  }

  // uncomment these lines if on Apple OS X
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow((int)scene->options.resolution.x,
                            (int)scene->options.resolution.y, "rt", 0, 0);

  if (!window) {
    fprintf(stdout, "ERROR: could not open window with GLFW3\n");
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  init_renderer();
}

void App::init_renderer() {
  renderer = new Renderer(scene);
  renderer->init();
}

void App::render() {
  renderer->render();
  const glm::ivec2 screenSize = renderer->screen;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, screenSize.x, screenSize.y);
  renderer->draw();
  glfwSwapBuffers(window);
}

void App::run() {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    auto err = glGetError();
    if (err != GL_NO_ERROR) {
      std::cout << "OpenGL error: " << err << "\n";
      glfwTerminate();
    }

    renderer->update(0.1);
    render();
  }
}

void App::finish() {
  delete renderer;
  glfwTerminate();
}
