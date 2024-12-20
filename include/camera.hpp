#pragma once
#include <glm/glm.hpp>

class Camera {
public:
  Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov,
         float aspect, float aperture, float focus_dist);
  // Camera(const Camera &other);

  glm::vec3 origin, llc, horizontal, vertical, u, v, w;
  float lens_radius;

  glm::vec3 lookfrom, lookat;
  float vfov, aperture, focus_dist;

  void update();

private:
  glm::vec3 vup;
  float aspect;
};