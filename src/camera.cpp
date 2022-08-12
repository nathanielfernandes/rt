#include <camera.hpp>

Camera::Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov,
               float aspect, float aperture, float focus_dist) {
  float theta = vfov * M_PI / 180.0f;
  float half_height = tan(theta / 2.0f);
  float half_width = aspect * half_height;

  origin = lookfrom;

  w = glm::normalize(lookfrom - lookat);
  u = glm::normalize(glm::cross(vup, w));
  v = glm::cross(w, u);

  llc = origin - half_width * focus_dist * u - half_height * focus_dist * v -
        focus_dist * w;

  horizontal = 2.0f * half_width * focus_dist * u;
  vertical = 2.0f * half_height * focus_dist * v;

  lens_radius = aperture / 2.0f;
}
