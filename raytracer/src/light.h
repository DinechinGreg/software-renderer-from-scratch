#pragma once

#include <math/vec.h>
#include <transform.h>

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
    light(light_type type, float intensity, vec3f const& position_or_direction = vec3f::zero(), vec3f const& color = vec3f::one());
    ~light() = default;
    light(light const& other) = default;
    light& operator=(light const& other) = default;

    light_type const& get_type() const { return m_type; }
    float const& get_intensity() const { return m_intensity; }
    vec3f const& get_direction() const { return m_direction; }

    float apply_lighting(vec3f const& point_position, unit_vec3f const& point_normal, unit_vec3f const& view_direction, float const& specular_intensity) const;

  protected:
    light_type m_type;
    float m_intensity;
    vec3f m_color;
    unit_vec3f m_direction;
};
