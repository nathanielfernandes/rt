#include <glu.hpp>

#include <GLFW/glfw3.h>
#include <renderer.hpp>

class App {
public:
  Scene *scene;

  App(Scene *scene);
  ~App();

  void init();
  void init_renderer();
  void run();
  void finish();

private:
  Renderer *renderer;
  GLFWwindow *window;
  bool initialized;

  void render();

  static void glfw_error_callback(int error, const char *description) {
    fprintf(stdout, "GLFW Error: %s\n", description);
    exit(1);
  }
};