#include "light.h"

#include <graphics/renderer/renderer.h>
#include <math/math.h>
#include <math/vec.h>

#include <cmath>

Light::Light()
    : Light{Light_Type::ambient, 0.0f}
{
}

Light::Light(Light_Type type, float intensity, Vec3f const& position_or_direction, Vec3f const& color)
    : Transform{position_or_direction}
    , m_type{type}
    , m_intensity{intensity}
    , m_color{color}
    , m_direction{Unit_Vec3f::zero()}
{
    if (type == Light_Type::directional)
    {
        m_position = Vec3f::zero();
        m_direction = Unit_Vec3f{position_or_direction};
    }
}

Unit_Vec3f Light::compute_light_direction(Vec3f const& point_position) const
{
    if (m_type == Light_Type::point)
    {
        auto const point_to_light = (m_position - point_position);
        return point_to_light.normalize();
    }
    else if (m_type == Light_Type::directional)
    {
        return -m_direction;
    }
    return Vec3f::zero();
}

float Light::compute_light_distance(Vec3f const& point_position) const
{
    if (m_type == Light_Type::point)
    {
        auto const point_to_light = (m_position - point_position);
        return point_to_light.length();
    }
    else if (m_type == Light_Type::directional)
    {
        return math::numeric_infinity();
    }
    return 0.0f;
}

Vec3f Light::compute_radiance(float const& light_distance) const
{
    float attenuation = 1.0f;
    if (m_type == Light_Type::point)
        attenuation = 1.0f / (light_distance * light_distance);
    auto const radiance = m_intensity * m_color * attenuation;
    return radiance;
}
