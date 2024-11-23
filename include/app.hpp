#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glu.hpp>

#include <GLFW/glfw3.h>
#include <renderer.hpp>

class App {
public:
  Scene *scene;

  App(Scene *scene);
  ~App();

  void init_renderer();
  void init_gui();
  void gui();
  void run();
  void finish();

private:
  Renderer *renderer;
  GLFWwindow *window;
  bool initialized;
  bool moving;

  void render();

  static void glfw_error_callback(int error, const char *description);

  static void glfw_key_callback(GLFWwindow *window, int key, int scancode,
                                int action, int mods);

  void handle_input(float dt);
  bool key_pressed(int key);
};