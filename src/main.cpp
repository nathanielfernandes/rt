#include "load.hpp"
#include <app.hpp>
#include <iostream>

auto main() -> int {
  Scene *scene = new Scene();
  scene->options = Options();

  Camera camera =
      Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
             glm::vec3(0.0f, 1.0f, 0.0f), 0.0, 0.0, 0.0, 0.0);

  scene->setCamera(&camera);

  if (!LoadModel(scene, "assets/teapot.obj")) {
    std::cout << "Failed to load model" << std::endl;
    return 1;
  }

  // print amount of triangles
  std::cout << "Amount of triangles: " << scene->triangles.size() << std::endl;

  App app(scene);

  app.init();

  app.run();

  app.finish();
}
