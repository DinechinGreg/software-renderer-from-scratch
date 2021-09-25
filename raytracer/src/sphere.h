#pragma once

#include <math/vec.h>
#include <transform.h>

#include <vector>

class sphere : public transform
{
  public:
    sphere();
    sphere(vec3f const& center, float radius, vec3f const& color, float const& specular_intensity, float const& reflective_intensity);
    ~sphere() = default;
    sphere(sphere const& other) = default;
    sphere& operator=(sphere const& other) = default;

    vec3f const& get_color() const { return m_color; }
    float const& get_specular_intensity() const { return m_specular_intensity; }
    float const& get_reflective_intensity() const { return m_reflective_intensity; }

    bool intersects_ray(vec3f const& origin, unit_vec3f const& ray, std::vector<float>& intersections) const;

  private:
    float m_radius;
    vec3f m_color;
    float m_specular_intensity;
    float m_reflective_intensity;
};
