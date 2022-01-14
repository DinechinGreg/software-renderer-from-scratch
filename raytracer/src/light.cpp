#include "light.h"

#include <math/geometry.h>
#include <math/math.h>
#include <math/vec.h>
#include <renderer/renderer.h>

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

float Light::apply_lighting(Vec3f const& point_position, float point_distance, Unit_Vec3f const& point_normal, Unit_Vec3f const& view_direction, float const& specular_intensity) const
{
    float out_lighting = 0.0f;
    // Apply ambient lighting
    if (m_type == Light_Type::ambient)
    {
        out_lighting += m_intensity;
    }
    else
    {
        Unit_Vec3f l{Vec3f::zero()};
        float max_intersection_distance{0.0f};
        if (m_type == Light_Type::point)
        {
            auto const point_to_light = (m_position - point_position);
            l = point_to_light.normalize();
            max_intersection_distance = point_to_light.length();
        }
        else if (m_type == Light_Type::directional)
        {
            l = -m_direction;
            max_intersection_distance = math::numeric_infinity();
        }

        // Check for shadows
        if (Renderer::get_instance().intersects_any_sphere(point_position, l, math::distance_epsilon(point_distance, 1.0f, 1e-2f), max_intersection_distance))
            return out_lighting;

        // Apply diffuse lighting
        auto const& n_dot_l = point_normal.dot(l);
        if (n_dot_l > 0.0f)
        {
            out_lighting += m_intensity * n_dot_l;
        }

        // Apply specular lighting
        if (specular_intensity != -1)
        {
            auto const& reflection_direction = math::compute_reflected_ray(l, point_normal, &n_dot_l).normalize();
            auto const& r_dot_v = reflection_direction.dot(view_direction);
            if (r_dot_v > 0)
            {
                out_lighting += m_intensity * std::pow(r_dot_v, specular_intensity);
            }
        }
    }
    return out_lighting;
}
