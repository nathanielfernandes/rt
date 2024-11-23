#include <camera.hpp>
#include <iostream>

Camera::Camera(glm::vec3 lookfrom_, glm::vec3 lookat_, glm::vec3 vup_,
               float vfov_, float aspect_, float aperture_, float focus_dist_) {

  lookfrom = lookfrom_;
  lookat = lookat_;
  vup = vup_;
  vfov = vfov_;
  aspect = aspect_;
  aperture = aperture_;
  focus_dist = focus_dist_;

  update();

  //   float theta = vfov * M_PI / 180.0f;
  //   float half_height = tan(theta / 2.0f);
  //   float half_width = aspect * half_height;

  //   origin = lookfrom;

  //   w = glm::normalize(lookfrom - lookat);
  //   u = glm::normalize(glm::cross(vup, w));
  //   v = glm::cross(w, u);

  //   llc = origin - half_width * focus_dist * u - half_height * focus_dist * v
  //   -
  //         focus_dist * w;

  //   horizontal = 2.0f * half_width * focus_dist * u;
  //   vertical = 2.0f * half_height * focus_dist * v;

  //   lens_radius = aperture / 2.0f;
}

void Camera::update() {
  float theta = vfov * M_PI / 180.0f;
  float half_height = tan(theta / 2.0f);
  float half_width = aspect * half_height;

  origin = lookfrom;

  //   std::cout << lookat[] << "\n";

  w = glm::normalize(lookfrom - lookat);
  u = glm::normalize(glm::cross(vup, w));
  v = glm::cross(w, u);

  llc = origin - half_width * focus_dist * u - half_height * focus_dist * v -
        focus_dist * w;

  horizontal = 2.0f * half_width * focus_dist * u;
  vertical = 2.0f * half_height * focus_dist * v;

  lens_radius = aperture / 2.0f;
}