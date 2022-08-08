#include <app.hpp>
#include <iostream>

App::App() {}
App::~App() {}

void App::init(const char *title, int xpos, int ypos, int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

    std::cout << "SDL Initialized!" << std::endl;

    window = SDL_CreateWindow(title, xpos, ypos, width, height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (window) {
      std::cout << "Window created!" << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      std::cout << "Renderer created!" << std::endl;
    }

    is_running = true;
  } else {
    is_running = false;
  }
}

void App::handle_events() {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    is_running = false;
  }
}

void App::render() {
  SDL_RenderClear(renderer);
  // render stuff here

  SDL_RenderPresent(renderer);
}

void App::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  std::cout << "Cleaned up!" << std::endl;
}

void App::update() {
  // update stuff here
}