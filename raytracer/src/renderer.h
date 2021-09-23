#pragma once

#include <camera.h>
#include <sphere.h>
#include <vec3.h>

#include <array>

class renderer
{
  public:
    static renderer &get_instance()
    {
        static renderer global_renderer;
        return global_renderer;
    }

    void setup_scene();
    vec3<float> const compute_color_from_ray(vec3<float> const &origin, vec3<float> const &direction, float near,
                                             float far);
    vec3<float> const compute_pixel_color(camera const &draw_camera, float u, float v);
    void draw_to_file(camera const &draw_camera);

  private:
    renderer() = default;
    ~renderer() = default;
    renderer(renderer const &other) = delete;
    renderer &operator=(renderer const &other) = delete;

    std::array<sphere, 3> m_spheres;
};
