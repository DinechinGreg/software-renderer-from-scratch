#include "light.h"

#include <renderer.h>
#include <unit_vec3.h>
#include <vec3.h>

#include <cmath>

light::light()
    : light{light_type::ambient, 0.0f}
{
}

light::light(light_type type, float intensity, vec3<float> const& position_or_direction, vec3<float> const& color)
    : transform{position_or_direction}
    , m_type{type}
    , m_intensity{intensity}
    , m_color{color}
    , m_direction{unit_vec3<float>::zero()}
{
    if (type == light_type::directional)
    {
        m_position = vec3<float>::zero();
        m_direction = unit_vec3{position_or_direction};
    }
}

float light::apply_lighting(vec3<float> const& point_position, unit_vec3<float> const& point_normal, unit_vec3<float> const& view_direction, float const& specular_intensity) const
{
    float out_lighting = 0.0f;
    // Apply ambient lighting
    if (m_type == light_type::ambient)
    {
        out_lighting += m_intensity;
    }
    else
    {
        unit_vec3<float> l{1.0f};
        float max_intersection_distance{0.0f};
        if (m_type == light_type::point)
        {
            auto const point_to_light = (m_position - point_position);
            l = point_to_light.normalize();
            max_intersection_distance = point_to_light.length();
        }
        else if (m_type == light_type::directional)
        {
            l = -m_direction;
            max_intersection_distance = std::numeric_limits<float>::infinity();
        }

        // Check for shadows
        auto const closest_sphere_intersection = renderer::get_instance().compute_closest_sphere_intersection(point_position, l, std::numeric_limits<float>::epsilon(), max_intersection_distance);
        if (closest_sphere_intersection.first != nullptr)
            return 0.0f;

        // Apply diffuse lighting
        auto const& n_dot_l = point_normal.dot(l);
        if (n_dot_l > 0.0f)
        {
            out_lighting += m_intensity * n_dot_l;
        }

        // Apply specular lighting
        if (specular_intensity != -1)
        {
            auto const& reflection_direction = renderer::get_instance().reflect_ray(l, point_normal, &n_dot_l).normalize();
            auto const& r_dot_v = reflection_direction.dot(view_direction);
            if (r_dot_v > 0)
            {
                out_lighting += m_intensity * std::pow(r_dot_v, specular_intensity);
            }
        }
    }
    return out_lighting;
}
