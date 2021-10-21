#pragma once

#include <math/vec.h>
#include <transform.h>

#include <limits>
#include <vector>

class sphere : public transform
{
  public:
    sphere();
    sphere(vec3f const& center, float radius, vec3f const& color, float const& specular_intensity, float const& reflective_intensity);
    ~sphere() = default;
    sphere(sphere const& other) = default;
    sphere& operator=(sphere const& other) = default;

    float get_radius() const { return m_radius; }
    vec3f const& get_color() const { return m_color; }
    float get_specular_intensity() const { return m_specular_intensity; }
    float get_reflective_intensity() const { return m_reflective_intensity; }

  private:
    float m_radius;               // Radius of the sphere
    vec3f m_color;                // Color of the sphere
    float m_specular_intensity;   // Specular intensity of the sphere
    float m_reflective_intensity; // Reflective intensity of the sphere
};
