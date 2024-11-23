#include "data.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <app.hpp>
#include <iostream>

void App::glfw_error_callback(int error, const char *description) {
  std::cout << "GLFW error: " << description << std::endl;
  exit(1);
}

void App::glfw_key_callback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
      // glfwSetWindowShouldClose(window, GLFW_TRUE);
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

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  // uncomment these lines if on Apple OS X
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
  glfwSetErrorCallback(glfw_error_callback);

  window = glfwCreateWindow((int)scene->options.resolution.x,
                            (int)scene->options.resolution.y, "rt", 0, 0);

  if (!window) {
    fprintf(stdout, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  init_gui();

  glfwSetKeyCallback(window, glfw_key_callback);

  init_renderer();
}

App::~App() {
  if (initialized) {
    finish();
  }
}

void App::init_gui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  io.Fonts->AddFontFromFileTTF("assets/Roboto-Bold.ttf", 16);

  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  ImGui::StyleColorsDark();

  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowRounding = 5.3f;
  style.FrameRounding = 2.3f;
  style.ScrollbarRounding = 0;

  style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] =
      ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered] =
      ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
}

void App::init_renderer() { renderer = new Renderer(scene); }

void App::render() {
  renderer->render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glViewport(0, 0, renderer->resolution.x, renderer->resolution.y);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  renderer->draw();

  gui();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void App::gui() {
  bool updated = false;

  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar |
      ImGuiWindowFlags_NoTitleBar;

  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("InvisibleWindow", nullptr, windowFlags);

  ImGui::BeginMenuBar();
  ImGui::TextColored(ImVec4(1.0, 0.8, 0.0, 1.0), "nathanielfernandes/rt");

  if (ImGui::BeginMenu("File")) {
    if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
    }
    if (ImGui::MenuItem("Close", "Ctrl+W")) {
    }
    ImGui::EndMenu();
  }
  ImGui::EndMenuBar();

  ImGui::PopStyleVar(3);

  ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

  ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f),
                   ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::End();

  ImGuiIO io = ImGui::GetIO();

  ImGui::Begin("Debug", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImGui::PushItemWidth(200);
  ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f),
                     "FPS: %0.f (%0.2fms/frame)", io.Framerate,
                     1000 / io.Framerate);

  ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Samples %d",
                     renderer->sampleCount);

  if (ImGui::SliderInt("Max Bounces", &renderer->depthMax, 1, 50))
    updated = true;

  if (ImGui::Button("Reload"))
    updated = true;

  ImGui::End();

  ImGui::Begin("Scene Materials");
  const auto mat_names = scene->material_names;
  const auto mats = scene->materials;
  bool reload_mats = false;

  for (int i = 0; i < mat_names.size(); i++) {
    ImGui::PushItemWidth(200);
    if (ImGui::TreeNode(mat_names[i].c_str())) {

      int mat_type = (int)mats[i].materialType;

      std::string mt;
      switch (mat_type) {
      case 0:
        mt = "Lambertian";
        break;
      case 1:
        mt = "Metal";
        break;
      case 2:
        mt = "Dielectric";
        break;
      case 3:
        mt = "Emissive";
        break;
      }

      if (ImGui::ColorEdit4("Albedo", &scene->materials[i].albedo[0],
                            ImGuiColorEditFlags_PickerHueWheel))
        reload_mats = true;

      if (ImGui::SliderInt(("Material: " + mt).c_str(), &mat_type, 0, 3)) {
        scene->materials[i].materialType = (float)mat_type;
        reload_mats = true;
      }

      if (mat_type == 1 &&
          ImGui::SliderFloat("Fuzz", &scene->materials[i].fuzz, 0.0, 1.0))
        reload_mats = true;

      if (mat_type == 2 &&
          ImGui::SliderFloat("IOR", &scene->materials[i].ir, 0.0, 5.0))
        reload_mats = true;

      ImGui::TreePop();
    }
  }
  ImGui::End();

  ImGui::Begin("Camera Settings");
  bool update_cam = false;

  if (ImGui::DragFloat3("Look From", &scene->camera->lookfrom[0], 0.1, -1000.0,
                        1000.0))
    update_cam = true;

  if (ImGui::DragFloat3("Look At", &scene->camera->lookat[0], 0.1, -1000.0,
                        1000.0))
    update_cam = true;

  if (ImGui::DragFloat("Aperture", &scene->camera->aperture, 0.01, 0.0, 1.0))
    update_cam = true;

  if (ImGui::DragFloat("Focal Length", &scene->camera->focus_dist, 0.01, 0.0,
                       1000.0))
    update_cam = true;

  ImGui::End();

  if (update_cam) {
    scene->camera->update();
    updated = true;
  }

  if (reload_mats) {
    renderer->reloadMaterials();
    updated = true;
  }

  if (updated)
    renderer->reloadAcc();
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
  // if (dt > 2.0) {

  // renderer->depthMax = renderer->scene->options.depth;
  // }

  // move left
  if (key_pressed(GLFW_KEY_A)) {
    renderer->scene->camera->lookfrom.x -= 0.05f;
    // renderer->scene->camera->lookat.x -= 0.05f;
  }
  // move right
  if (key_pressed(GLFW_KEY_D)) {
    renderer->scene->camera->lookfrom.x += 0.05f;
    // renderer->scene->camera->lookat.x += 0.05f;
  }
  // move up
  if (key_pressed(GLFW_KEY_SPACE)) {
    renderer->scene->camera->lookfrom.y += 0.05f;
    // renderer->scene->camera->lookat.y += 0.05f;
  }
  // move down
  if (key_pressed(GLFW_KEY_LEFT_SHIFT)) {
    renderer->scene->camera->lookfrom.y -= 0.05f;
    // renderer->scene->camera->lookat.y -= 0.05f;
  }
  // move forward
  if (key_pressed(GLFW_KEY_W)) {
    renderer->scene->camera->lookfrom.z -= 0.05f;
    // renderer->scene->camera->lookat.z -= 0.05f;
  }
  // move backward
  if (key_pressed(GLFW_KEY_S)) {
    renderer->scene->camera->lookfrom.z += 0.05f;
    // renderer->scene->camera->lookat.z += 0.05f;
  }

  if (moving) {
    moving = false;
    // renderer->depthMax = 1;
    scene->camera->update();
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
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
}
