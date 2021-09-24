#pragma once

#include <transform.h>
#include <unit_vec3.h>
#include <vec3.h>

enum class light_type
{
    ambient,
    directional,
    point
};

class light : public transform
{
  public:
    light();
    light(light_type type, float intensity, vec3<float> const& position_or_direction = vec3<float>::zero(), vec3<float> const& color = vec3<float>::one());
    ~light() = default;
    light(light const& other) = default;
    light& operator=(light const& other) = default;

    light_type const& get_type() const { return m_type; }
    float const& get_intensity() const { return m_intensity; }
    vec3<float> const& get_direction() const { return m_direction; }

    float apply_lighting(vec3<float> const& point_position, unit_vec3<float> const& point_normal, unit_vec3<float> const& view_direction, float const& specular_intensity) const;

  protected:
    light_type m_type;
    float m_intensity;
    vec3<float> m_color;
    unit_vec3<float> m_direction;
};
