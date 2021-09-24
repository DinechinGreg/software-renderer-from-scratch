#pragma once

#include <camera.h>
#include <light.h>
#include <sphere.h>
#include <unit_vec3.h>
#include <vec3.h>

#include <vector>

class renderer
{
  public:
    static renderer& get_instance()
    {
        static renderer global_renderer{vec3<float>::zero()};
        return global_renderer;
    }

    void setup_scene();

    vec3<float> const reflect_ray(vec3<float> const& ray, unit_vec3<float> const& around, float const* precomputed_dot = nullptr) const;
    std::pair<sphere const*, float> compute_closest_sphere_intersection(vec3<float> const& origin, unit_vec3<float> const& direction, float near, float far) const;
    float compute_lighting(vec3<float> const& position, vec3<float> const& normal, vec3<float> const& view_direction, float specular_intensity) const;
    vec3<float> const compute_color_from_ray(vec3<float> const& origin, unit_vec3<float> const& direction, float near, float far, float recursion_depth) const;
    vec3<float> const compute_pixel_color(camera const& draw_camera, float u, float v) const;
    void draw_to_file(camera const& draw_camera) const;

  private:
    renderer(vec3<float> const& background_color);
    ~renderer() = default;
    renderer(renderer const& other) = delete;
    renderer& operator=(renderer const& other) = delete;

    std::vector<sphere> m_spheres;
    std::vector<light> m_lights;
    vec3<float> m_background_color;
};
