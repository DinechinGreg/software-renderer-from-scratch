#pragma once

#include <transform.h>
#include <vec3.h>

#include <vector>

class sphere : public transform
{
  public:
    sphere();
    sphere(vec3<float> const& center, float radius, vec3<float> const& color, float const& specular_intensity, float const& reflective_intensity);
    ~sphere() = default;
    sphere(sphere const& other) = default;
    sphere& operator=(sphere const& other) = default;

    vec3<float> const& get_color() const { return m_color; }
    float const& get_specular_intensity() const { return m_specular_intensity; }
    float const& get_reflective_intensity() const { return m_reflective_intensity; }

    bool intersects_ray(vec3<float> const& origin, unit_vec3<float> const& ray, std::vector<float>& intersections) const;

  private:
    float m_radius;
    vec3<float> m_color;
    float m_specular_intensity;
    float m_reflective_intensity;
};
