#include <SDL2/SDL.h>
#include <app.hpp>
#include <vec3.hpp>

#define WIDTH 1920
#define HEIGHT 1080

App *app = nullptr;

auto main() -> int {
  app = new App();

  app->init("SDL2 Window \"rt\"", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT);

  while (app->running()) {
    app->handle_events();
    app->update();
    app->render();
  }

  app->clean();

  return 0;
}