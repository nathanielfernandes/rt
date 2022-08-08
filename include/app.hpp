#ifndef App_hpp
#define App_hpp

#include <SDL2/SDL.h>

class App {
public:
  App();
  ~App();

  void init(const char *title, int xpos, int ypos, int width, int height);

  void handle_events();
  void update();
  void render();
  void clean();

  bool running() { return is_running; }

private:
  bool is_running;
  SDL_Window *window;
  SDL_Renderer *renderer;
};

#endif /* App_hpp */