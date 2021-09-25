#pragma once

#include <camera.h>
#include <light.h>
#include <math/vec.h>
#include <sphere.h>

#include <vector>

class renderer
{
  public:
    static renderer& get_instance()
    {
        static renderer global_renderer{vec3f::zero()};
        return global_renderer;
    }

    void setup_scene();

    vec3f const reflect_ray(vec3f const& ray, unit_vec3f const& around, float const* precomputed_dot = nullptr) const;
    std::pair<sphere const*, float> compute_closest_sphere_intersection(vec3f const& origin, unit_vec3f const& direction, float near, float far) const;
    float compute_lighting(vec3f const& position, vec3f const& normal, vec3f const& view_direction, float specular_intensity) const;
    vec3f const compute_color_from_ray(vec3f const& origin, unit_vec3f const& direction, float near, float far, float recursion_depth) const;
    vec3f const compute_pixel_color(camera const& draw_camera, float u, float v) const;
    void draw_to_file(camera const& draw_camera) const;

  private:
    renderer(vec3f const& background_color);
    ~renderer() = default;
    renderer(renderer const& other) = delete;
    renderer& operator=(renderer const& other) = delete;

    std::vector<sphere> m_spheres;
    std::vector<light> m_lights;
    vec3f m_background_color;
};
