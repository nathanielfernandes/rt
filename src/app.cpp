#include <app.hpp>
#include <iostream>

void App::glfw_error_callback(int error, const char *description) {
  std::cout << "GLFW error: " << description << std::endl;
}

void App::glfw_key_callback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;
      //     case GLFW_KEY_A:
      //       *renderer->scene->camera->origin.x -= 0.1f;

      //       break;
    }
  }
}

App::App(Scene *scene) : scene(scene) {
  if (!glfwInit()) {
    fprintf(stdout, "ERROR: could not start GLFW3\n");
    return;
  }

  // uncomment these lines if on Apple OS X
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);

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

  // glfwSetErrorCallback(glfw_error_callback);
  glfwSetKeyCallback(window, glfw_key_callback);

  init_renderer();
}

App::~App() {
  if (initialized) {
    finish();
  }
}

void App::init_renderer() { renderer = new Renderer(scene); }

void App::render() {
  renderer->render();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, renderer->resolution.x, renderer->resolution.y);

  renderer->draw();

  glfwSwapBuffers(window);
}

bool App::key_pressed(int key) {
  int state = glfwGetKey(window, key);
  if (state == GLFW_PRESS || state == GLFW_REPEAT) {
    moving = true;
    return true;
  }
  return false;
}

void App::handle_input(float dt) {
  if (dt > 2.0) {
    moving = false;
    // renderer->depthMax = renderer->scene->options.depth;
  }

  // move left
  if (key_pressed(GLFW_KEY_A)) {
    renderer->scene->camera->origin.x -= 0.05f;
  }
  // move right
  if (key_pressed(GLFW_KEY_D)) {
    renderer->scene->camera->origin.x += 0.05f;
  }
  // move up
  if (key_pressed(GLFW_KEY_SPACE)) {
    renderer->scene->camera->origin.y += 0.05f;
  }
  // move down
  if (key_pressed(GLFW_KEY_LEFT_SHIFT)) {
    renderer->scene->camera->origin.y -= 0.05f;
  }
  // move forward
  if (key_pressed(GLFW_KEY_W)) {
    renderer->scene->camera->origin.z -= 0.05f;
  }
  // move backward
  if (key_pressed(GLFW_KEY_S)) {
    renderer->scene->camera->origin.z += 0.05f;
  }

  if (moving) {
    // renderer->depthMax = 1;
    renderer->reloadAcc();
  }
}

void App::run() {
  moving = false;
  float lastTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    float currentTime = glfwGetTime();

    auto err = glGetError();
    if (err != GL_NO_ERROR) {
      std::cout << "OpenGL error: " << err << "\n";
      glfwTerminate();
    }

    handle_input(currentTime - lastTime);

    if (!moving) {
      lastTime = currentTime;
    }

    renderer->update(currentTime);
    render();
  }
}

void App::finish() {
  // delete renderer;
  glfwTerminate();
}
