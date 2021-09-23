#include "light.h"

#include <cmath>

light::light() : light{light_type::ambient, 0.0f}
{
}

light::light(light_type type, float intensity, vec3<float> const &position_or_direction, vec3<float> const &color)
    : transform{position_or_direction}, m_type{type}, m_intensity{intensity}, m_color{color}, m_direction{vec3{0.0f}}
{
    if (type == light_type::directional)
    {
        m_position = vec3{0.0f};
        m_direction = position_or_direction;
    }
}

float light::apply_lighting(vec3<float> const &point_position, vec3<float> const &point_normal,
                            vec3<float> const &view_direction, float const &specular_intensity) const
{
    float out_lighting = 0.0f;
    // Apply ambient lighting
    if (m_type == light_type::ambient)
    {
        out_lighting += m_intensity;
    }
    else
    {
        // Apply diffuse lighting
        auto l = vec3<float>{0.0f};
        if (m_type == light_type::point)
        {
            l = m_position - point_position;
        }
        else if (m_type == light_type::directional)
        {
            l = -m_direction;
        }
        l = l.normalize();
        auto const &n_dot_l = point_normal.dot(l);
        if (n_dot_l > 0.0f)
        {
            out_lighting += m_intensity * n_dot_l;
        }

        // Apply specular lighting
        if (specular_intensity != -1)
        {
            auto const &reflection_direction = (2.0f * n_dot_l * point_normal - l).normalize();
            auto const &r_dot_v = reflection_direction.dot(view_direction);
            if (r_dot_v > 0)
            {
                out_lighting += m_intensity * std::pow(r_dot_v, specular_intensity);
            }
        }
    }
    return out_lighting;
}
